#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>
#include <semaphore.h>

/************************************** DATA ************************************/

const char* inFilename;
int fdInput;
int inSize;

char* inFile;
int* inEntry;

const char* outFilename;
int fdOutput;


/*************************************** INPUT **********************************/

char smallLetter(char c) { return 'a' <= c && c <= 'z'; }
char bigLetter(char c) { return 'A' <= c && c <= 'Z'; }
char newLine(char c) { return c == '\n'; }

int nStrings;

void getNumberOfStrings()
{
    nStrings = 0;
    for (int i = 0; i < inSize; ++i) {
        if (newLine(inFile[i])) {
            ++nStrings;
        }
    }
}

void openInFile()
{
	fdInput = open(inFilename, O_RDONLY);
	if (fdInput < 0) {
		printf("Failed to open %s\n", inFilename);
		exit(0);
	}

    // get size by shifting to end
    lseek(fdInput, 0L, SEEK_END);
    inSize = lseek(fdInput, 0L, SEEK_CUR);
    lseek(fdInput, 0L, SEEK_SET);

	inFile = (char*) mmap(NULL, inSize, PROT_READ, MAP_SHARED, fdInput, 0);

    getNumberOfStrings();
}

void closeInFile()
{
    munmap(inFile, inSize);
    close(fdInput);
}

/************************************** SHARED MEMORY **********************/

int fdAllSpace;
int allSpaceSize;
void* allSpace;

char* usedEntry;
int* lastStr;
int* lastOutPos;

int* startPointer;
int* endPointer;

char* mainSpace;

sem_t* semData;

void createSharedMemory()
{
	/* creating shared memory for OUTPUT */
	fdAllSpace = shm_open("/all_space", O_RDWR | O_CREAT, S_IRWXU);
    assert(allSpace >= 0 && "Create shared memory by shm_open()");

    allSpaceSize = 2 * sizeof(int) +                    // lastStr, lastOutPos
                       2 * nStrings * sizeof(int) +    // output strings pointers
                       2 * inSize * sizeof(char) +      // mainSpace 
                       nStrings * sizeof(char);

    assert( ftruncate(fdAllSpace, allSpaceSize) == 0 );

	allSpace = mmap(NULL, allSpaceSize,
                    PROT_WRITE | PROT_READ,
                    MAP_SHARED, fdAllSpace, 0);

	lastStr = allSpace;
    lastOutPos = (void*) lastStr + sizeof(int);

    startPointer = (void*) lastOutPos + sizeof(int);
    endPointer = (void*) startPointer + nStrings * sizeof(int);

    mainSpace = (void*) endPointer + nStrings * sizeof(int);

    usedEntry = (void*) mainSpace + 2 * inSize * sizeof(char);
    memset(usedEntry, 0, nStrings * sizeof(char));

	*lastStr = 0;
    *lastOutPos = 0;

    semData = sem_open("my_output", O_CREAT, 0600, 0);
    assert(semData != SEM_FAILED && "Semaphore creation should work");
    sem_post(semData); // set init value
}

void closeSharedMemory()
{
    sem_unlink("my_output");
    munmap(allSpace, allSpaceSize);
    close(fdAllSpace);
	shm_unlink("/output_string");
}

/************************************** PROCESS ***********************************/

// set entry positions of 
// input and output strings
void setEntries()
{
    inEntry = malloc(nStrings * sizeof(int));
    inEntry[0] = 0;

    int strIdx = 0;
    for (int idx = 0; idx < inSize; ++idx) {
        if (newLine(inFile[idx])) {
            ++strIdx;
            inEntry[strIdx] = idx + 1;
        }
    }
}

void childProcess()
{
    while (1) {
        int curString;

        /*************** block sem **********/
        sem_wait(semData);

        curString = *lastStr;

        ++*lastStr;
        while (*lastStr < nStrings && usedEntry[*lastStr]) {
            ++*lastStr;
        }

        fprintf(stderr, "INBLOCK child: %d, curString: %d, lastOutPos: %d\n", getpid(), curString, *lastOutPos);

        // nothing to process
        if (curString >= nStrings) {
            fprintf(stderr, "DIE\n");
            sem_post(semData);
            exit(0);
        }

        usedEntry[curString] = 1;

        int strLength = (curString + 1 == nStrings ? inSize : inEntry[curString + 1])
                            - inEntry[curString];

        startPointer[curString] = *lastOutPos;
        endPointer[curString] = *lastOutPos + 2 * strLength;
        *lastOutPos += 2 * strLength;


        sem_post(semData);
        /************* unblock sem ************/

        fprintf(stderr, "START child: %d, process: %d\n", getpid(), curString);

        int endPos = (curString + 1 == nStrings ? inSize : inEntry[curString + 1]);
        int startPos = inEntry[curString];
        fprintf(stderr, "child: %d, startPos: %d, endPos: %d\n", getpid(), startPos, endPos);
        int outPos = startPointer[curString];

        for (int inPos = startPos; inPos < endPos; ++inPos) {
            char c = inFile[inPos];
            fprintf(stderr, "curStr: %d, c: %c, child: %d\n", curString, c, getpid());
            if (smallLetter(c)) {
                /*mainSpace[outPos++] = toupper(c);*/
                mainSpace[outPos++] = c - 'a' + 'A';
            } else if (bigLetter(c)) {
                /*mainSpace[outPos++] = tolower(c);*/
                /*mainSpace[outPos++] = tolower(c);*/
                mainSpace[outPos++] = c - 'A' + 'a';
                mainSpace[outPos++] = c - 'A' + 'a';
            } else {
                mainSpace[outPos++] = c;
            }
        }
        fprintf(stderr, "len: %d, HERE!!!\n", outPos - startPointer[curString]);
        endPointer[curString] = outPos;
    }
}


const int nProcesses = 5;

void process()
{
    openInFile(); // also gets # of strings
    createSharedMemory();

    setEntries();

    fprintf(stderr, "All pre-work is done!\ninSize: %d\nnStrings: %d\n", inSize, nStrings);

    for (int i = 0; i < nProcesses; ++i) {
        if (!fork()) {
            fprintf(stderr, "Child: %d\n", getpid());
            childProcess();
            exit(0);
        }
    }
    for (int i = 0; i < nProcesses; ++i) {
        wait(NULL);
    }

    // gather output string
    for (int strIdx = 0; strIdx < nStrings; ++strIdx) {
        for (int i = startPointer[strIdx]; i < endPointer[strIdx]; ++i) {
            putchar(mainSpace[i]);
        }
    }

    closeSharedMemory();
    closeInFile();
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("No file\n");
		return 0;
	}

    inFilename = argv[1];
    fprintf(stderr, "%s\n", inFilename);
    process();

    return 0;
}
