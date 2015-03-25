#include "alloc.h"

// резервирует память
void * my_malloc(unsigned int size) {
    // Суть: мы должны найти пустой участок и отдать его

    // если хотим зарезервировать слишком много
    if(size > MY_ALLOC_PIECE_SIZE - sizeof(SPiece)) {
        perror("Out of memory...");
        return NULL;
    }

    // идём по всем участкам...
    int i;
    for(i = 0; i < piece_count; i++) {
        void * pieceBegin = piece_first + i * MY_ALLOC_PIECE_SIZE;
        SPiece * piece = (SPiece *) pieceBegin;

        // идём по буферам в поисках первого подходящего
        int j;
        for(j = 0; j < piece->used; j++) {
            if(piece->buffers[j].isFree == 1 && piece->buffers[j].size >= size) {
                printf("Found free compatible buffer (%d, %d): %d bytes\n", i, j, piece->buffers[j].size);

                // если этот буффер последний, то почему бы и не отдать его весь!
                if(piece->used == 16) {
                    piece->buffers[j].isFree = 0;
                    return piece->buffers[j].address;
                }

                // если мы тут, то можно ещё оторвать буффер от куска
                piece->buffers[1+j].isFree = 1;
                piece->buffers[1+j].size = piece->buffers[j].size - size;
                piece->buffers[1+j].address = piece->buffers[j].address + size;

                piece->buffers[j].isFree = 0;
                piece->buffers[j].size = size;

                piece->used++;

                return piece->buffers[j].address;
            }
        }
    }

    // если мы тут, то ничего не смогли найти.
    // что ж. иницилизируем один кусок.
    piece_create();
    void * pieceBegin = piece_first + (piece_count - 1) * MY_ALLOC_PIECE_SIZE;
    SPiece * piece = (SPiece *) pieceBegin;

    // отдадим первый кусок
    piece->buffers[1].isFree = 1;
    piece->buffers[1].size = piece->buffers[0].size - size;
    piece->buffers[1].address = piece->buffers[0].address + size;

    piece->buffers[0].isFree = 0;
    piece->buffers[0].size = size;

    piece->used++;

    printf("Found free compatible buffer (%d, 0): %d bytes\n", piece_count-1, piece->buffers[0].size);

    return piece->buffers[0].address;
}

// освобождает память
void my_free(void * mem) {
    // проверим, что этот кусина памяти реально наш
    if((mem < piece_first + sizeof(SPiece))
        || (mem > piece_first + piece_count * MY_ALLOC_PIECE_SIZE)) {
        perror("my_free: wrong pointer.");
        return;
    }

    // поиск, к какому куску относится
    int i;
    for(i = 0; i < piece_count; i++) {
        void * pieceBegin = piece_first + i * MY_ALLOC_PIECE_SIZE;
        SPiece * piece = (SPiece *) pieceBegin;

        // идём по буферам в поисках необходимого
        int j;
        for(j = 0; j < piece->used; j++) {
            if(piece->buffers[j].address == mem) {
                // ура! наш!
                piece->buffers[j].isFree = 1;
                buffers_reform();
                piece_autoclean();
                return;
            }
        }
    }
    perror("my_free: buffer not found.");
}

// перемещает память
void * my_realloc(void * mem, unsigned int size) {
    int oldSize = 0; // дляна найденного куска

    // проверим, что этот кусина памяти реально наш
    if((mem < piece_first + sizeof(SPiece))
        || (mem > piece_first + piece_count * MY_ALLOC_PIECE_SIZE)) {
        perror("my_realloc: wrong pointer.");
        return mem;
    }

    // высчитываем, к какому куску относится память
    void * pieceBegin = mem - ((mem - piece_first) % MY_ALLOC_PIECE_SIZE);
    SPiece * piece = (SPiece *) pieceBegin;

    // идём по буферам в поисках необходимого
    int j;
    for(j = 0; j < piece->used; j++) {
        if(piece->buffers[j].address == mem) {
            // мы его нашли!!

            if(piece->buffers[j].size >= size) {
                // халявка: мы уже выделили очень много
                puts("realloc: not needed");
                return mem;
            }

            oldSize = piece->buffers[j].size;
            piece->buffers[j].isFree = 1;
            break;
        }
    }

    // халявки не вышло: резервируем и переносим
    puts("realloc: needed");

    void * newMem = my_malloc(size);
    memcpy(newMem, mem, oldSize);

    buffers_reform();
    piece_autoclean();
    return newMem;
}
