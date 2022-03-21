#include <stdio.h>

//* (2) Dodaj do powyższego programu pętlę for idącą przez wszystkie wczytane liczby i drukującą te z nich, 
//* które są większe od 10 i mniejsze od 100.
void display(int arr[], int size)
{
    int i;
    for(i = 0; i<size; i++) {
        if(*(arr+i) > 10 && *(arr+i) < 100)
            printf("%d ", *(arr+i));
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    int liczby[50] = {0};
    int i = 0;

    // int liczba;
    // for(i = 0; i<50; i++)
    // {
    //     if(scanf("%d", &liczba) == EOF)
    //         break;
    //     if(liczba == 0)
    //         break;
        
    //     liczby[i] = liczba;
    // }

    while(scanf("%d", liczby+i) != EOF) {
        if(*(liczby+i) == 0 || i == 49)
            break;
        i++;
    } 

    display(liczby, i);
}