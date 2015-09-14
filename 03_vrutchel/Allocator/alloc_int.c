#include "alloc_int.h"

// создание куска памяти
void piece_create() {
    // allocate memory
    void * pBegin = sbrk(MY_ALLOC_PIECE_SIZE);
    if(pBegin <= 0) {
        perror("ERROR");
        exit(1);
    }

    // descriptor
    SPiece * pPiece = (SPiece *) pBegin;
    pPiece->used = 1;

    // init first buffer
    pPiece->buffers[0].isFree = 1;
    pPiece->buffers[0].size = MY_ALLOC_PIECE_SIZE - sizeof(SPiece);
    pPiece->buffers[0].address = pPiece + sizeof(SPiece);

    // increment counter
    piece_count++;
}

// переформирование буфферов - очистка пустых, которые стоят рядом
void buffers_reform() {
    int i;
    for(i = 0; i < piece_count; i++) {
        void * pieceBegin = piece_first + i * MY_ALLOC_PIECE_SIZE;
        SPiece * piece = (SPiece *) pieceBegin;

        // сортируем буффера в массиве по адрему
        qsort(piece->buffers, piece->used, sizeof(SBuffer), buffer_compare);

        int j;
        int oldUsed = piece->used;
        for(j = oldUsed - 1; j > 0; j--) {
            // если наш кусочек также свободен, как и предыдущий
            if(piece->buffers[j].isFree == 1 && piece->buffers[j-1].isFree == 1) {
                piece->buffers[j-1].size += piece->buffers[j].size;
                piece->buffers[j].size = 0;
                piece->used--;

                // сдвинем все предыдущие сюда
                int k;
                for(k = j+1; k < oldUsed; k++) {
                    piece->buffers[k-1] = piece->buffers[k];
                }
            }
        }
    }
}

// очищает последний кусок памяти, если все буфферы в нём не задействованы
void piece_autoclean() {
    int i;
    for(i = piece_count-1; i >= 0; i--) {
        void * pieceBegin = piece_first + i * MY_ALLOC_PIECE_SIZE;
        SPiece * piece = (SPiece *) pieceBegin;

        int j;
        for(j = 0; j < piece->used; j++) {
            if(piece->buffers[j].isFree == 0) {
                // дошли до первого непустого участка. остановимся
                return ;
            }
        }

        // таки удаляем этот кусок!
        sbrk(-1 * MY_ALLOC_PIECE_SIZE);
        piece_count--;
    }
}

// сравнение двух буфферов по адресу
int buffer_compare(const void* a, const void* b) {
    SBuffer* aBuffer = (SBuffer *) a;
    SBuffer* bBuffer = (SBuffer *) b;

    if(aBuffer->address > bBuffer->address) {
        return 1;
    }
    if(aBuffer->address < bBuffer->address) {
        return -1;
    }
    return 0;
}
