#include <stdio.h>
#include <unistd.h>
#include "Allocator.h"
#pragma once

#define FREE 1
#define FULL 0

Page *PageNum = NULL;


Page *CreatePage()
{
    Page *NewPage = Request(MEMORY);

    if (NewPage == NULL)
        return NULL;

    NewPage->next = NULL;
    NewPage->prev = NULL;
    NewPage->free = FREE;
    NewPage->size = MEMORY - sizeof(Page);

    return NewPage;
}

void *Request(unsigned Space)
{
    void *Allocated = sbrk(Space);

    if (Allocated == ((void *) -1)){
        fprintf(stderr, "Error, cannot move.\n");
        return NULL;
    }else{
        return Allocated;
    }
}


Page *LastPage(Page *PageNum)
{
    Page *Current = PageNum;

    if (PageNum == NULL)
        return NULL;

    while (Current->next != NULL)
        Current = Current->next;

    return Current;
}

void *Allocate(Page *page, unsigned Space)
{
    if (page->size - Space > sizeof(Page)){
        Page *NewPage = (Page*) ((char*) (page) + sizeof(Page) + Space);

        NewPage->next = page->next;
        NewPage->prev = page;
        NewPage->size = page->size - Space - sizeof(Page);
        NewPage->free = FREE;

        if (page->next != NULL){
            page->next->prev = NewPage;
        }

        page->next = NewPage;
        page->size = Space;

        MergeForward(NewPage);
    }

    page->free = FULL;

    return (char*)page + sizeof(Page);
}


void MergeForward(Page *page)
{
    if (page->next != NULL && page->next->free == FREE){
        page->size += page->next->size + sizeof(Page);

        if (page->next->next != NULL)
            page->next->next->prev = page;

        page->next = page->next->next;
    }
}

void MergeBackward(Page *page)
{
    if (page->prev != NULL && page->prev->free == FREE){
        page->prev->size += page->size + sizeof(Page);

        page->prev->next = page->next;

        if (page->next != NULL)
            page->next->prev = page->prev;
    }
}

Page *IncreasePage(Page *PageNum)
{
    Page *Last = LastPage(PageNum);
    Page *New = CreatePage();

    if (New == NULL)
        return NULL;

    if (Last == NULL)
        return New;

    Last->next = New;
    New->prev = Last;

    if (Last->free == FULL){
        return New;
    }else{
        MergeForward(Last);
        return Last;
    }
}

void *my_malloc(unsigned Space)
{
    printf("\033[0mAlloc: %d\033[0m\n", Space);

    if (PageNum == NULL)
    {
        PageNum = IncreasePage(PageNum);
    }

    Page *Current = PageNum;

    while (Current != NULL)
    {
        if (Current->free == FREE &&
            Current->size >= Space)
        {
            return Allocate(Current, Space);
        }

        Current = Current->next;
    }

    Page *Last = IncreasePage(PageNum);

    while (Last->size < Space)
    {
        if (IncreasePage(PageNum) == NULL)
            return NULL;
    }

    return Allocate(Last, Space);
}
