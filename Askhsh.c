#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE 3 // 3 pthseis 
const int SIZE_OF_INT = sizeof(int);

typedef struct Pthseis{
    int flight_id;
    char to_city[50];
    char from_city[50];
    int available_seats; //tyxaia
    float price;
}Pthseis;

void showflights(Pthseis flights[SIZE]);
void pthseis(Pthseis flights[SIZE]);
void InitA(int A[10][4]);
void changeA(int A[10][4]);
void seats (int fd[2], Pthseis flights[3], int* k, int Ep[3],float *total);

int main(int argc, char *argv[]) {

    srand(time(0)); //arxikopoihsh rologiou vash wra ekteleshs

    int fd[2]; //write - read
    float total = 0.00; //sunolika kerdh
    int A[10][4]; // pinakas me arithmous pthsewn, opou -1 = kamia pthsh
    int genplithos = 0; // sunolo krathsewn olwn twn pthsewn
    int genepituxeis = 0; // sunolo epithxhmenwn krathsewn 
    int Ep[3] = {0,0,0}; // pinakas me arithmo apo epituxeis krathseis giak kathe pthsh
    int plithos[3] = {0,0,0}; // pinakas me sunoliko arithmo krathsewn 

    Pthseis flights[3];

    InitA(A); //arxikopoioume ton pinaka A me -1 

    pipe(fd); //kanoume pipe ton fd 

    if (fork() == 0) { // 0 an eimaste sto child

        close(fd[0]); //kleinw to fd dhladh to read apo to child
        changeA(A);

        for(int i = 0 ; i < 10; i++){
            for(int j = 0; j < 4 ; j++){
                write(fd[1], &A[i][j], sizeof(A));  //grafw(metaferw) dedomena tou pinaka A apo to child sto parent
            }
        }
        close(fd[1]); //kleinw to write tou child
    }
    else {  //else = parent
        wait(NULL); //wait gia na ektelestei prwta to child process
        close(fd[1]);
        for(int i = 0 ; i < 10; i++){
            for(int j = 0; j < 4 ; j++){
                read(fd[0], &A[i][j], sizeof(A));  //read(lamvanw) dedomena tou pinaka A apo to child sto parent 
            }
        }

        pthseis(flights);

        int k; 

        for(int i = 0; i < 10; i++){

            printf("\nClient %d \n", i+1); 
            printf("arithmos eishthriwn   pthsh 1         pthsh 2         pthsh 3\n");
            for(int j = 0; j <4; j++){
                printf(" \t %d \t ", A[i][j]);
            }
            printf("\nSystem\n");
            if(A[i][0] == 1){ //an thelei na kleisei 1 eisitirio
            
                k = A[i][1]; // k = id pthshs
                seats(fd, flights, &k, Ep, &total);
            }
            else if(A[i][0] == 2){ //an thelei na kleisei 2 eisitiria

                k = A[i][1];
                seats(fd, flights, &k, Ep, &total);
                k = A[i][2];
                seats(fd, flights, &k, Ep, &total);
            }
            else if(A[i][0] == 3){ //an thelei na kleisei 3 eisitiria

                k = A[i][1];
                seats(fd, flights, &k, Ep, &total);
                k = A[i][2];
                seats(fd, flights, &k, Ep, &total);
                k = A[i][3];
                seats(fd, flights, &k, Ep, &total);
            }
            sleep(3);
        }

        for(int i = 0; i < 10; i++){  // geniko plithos krathsewn
            for(int j = 1; j < 4; j++){ //apo 1 gt den theloume thn 1h sthlh
                if(A[i][j]== 0){
                    plithos[0] = plithos[0] + 1;
                }
                else if(A[i][j]== 1){
                    plithos[1] = plithos[1] + 1;         
                }
                else if(A[i][j]==2){  // oxi else giati exoume kai -1
                    plithos[2] = plithos[2] + 1;
                }
            }
        }

        printf("\n ~SYGKENTRWTIKH ANAFORA~ \n");
        for(int i = 0; i < 3; i++){
            printf("\n");
            printf(" Apo : %s \n", flights[i].from_city);
            printf(" Pros : %s \n", flights[i].to_city);
            printf("%d sunolikes krathseis gia thn pthsh %d \n", plithos[i], i);
            genplithos = genplithos + plithos[i];
            printf("%d epituxeis krathseis \n", Ep[i]);
            genepituxeis = genepituxeis + Ep[i];
            int anepitixis = (plithos[i] - Ep[i]);
            printf("%d anepituxeis krathseis \n", anepitixis);
        }

        printf("\nSynoliko plithos krathsewn : %d\n", genplithos); //sygkentrwtiko mhnyma
        printf("Synolikes epituxeis krathseis : %d\n", genepituxeis);
        printf("Sunolika esoda etairias : %.2f\n", total);

        close(fd[0]);  //kleinw to read

    }
    return 0;
}

void changeA(int A[10][4]){ //gemisma pinaka A

    for(int i = 0; i < 10;i++){ // 10 pelates

        int random=0, random2=0; // random = id pthshs , random2 = arithmos eishthriwn pelath

        random = (rand() %3) + 1;             //dinei orisma apo 1 mexri 3
        A[i][0] = random;

        if(random == 1){                    //arithmos eishthriwn = 1
            random2 = rand() %SIZE;         //vriskei arithmo pthshs
            A[i][1] = random2;

        }
        else if(random == 2){
            random2 = rand() %SIZE;
            A[i][1] = random2;
            random2 = rand()%SIZE;
            A[i][2] = random2;
        }
        else if(random == 3){
            random2 = rand() %SIZE;
            A[i][1] = random2;
            random2 = rand()%SIZE;
            A[i][2] = random2;
            random2 = rand() %SIZE;
            A[i][3] = random2;
        }
    }
}

void InitA(int A[10][4]){ //arxikopoihsh pinaka A

    for(int i=0;i<10;i++){ //arxikopoihsh me -1
        for(int j=0; j<4; j++){
            A[i][j] = -1;
        }
    }

}

void pthseis(Pthseis flights[SIZE]){              //dhmiourgei tis pthseis

    srand(time(0)); //arxikopoihsh rologiou vash wra ekteleshs

    int r = 0;

    flights[0].flight_id = 0;
    strcpy(flights[0].from_city, "Athina");
    strcpy(flights[0].to_city, "Thessaloniki");
    flights[0].price = 59.99;
    r = (rand() % 5)+1; // orismata apo 1 mexri 5 gia tis theseis
    flights[0].available_seats = r;

    flights[1].flight_id = 1;
    strcpy(flights[1].from_city, "Ptolemaida");
    strcpy(flights[1].to_city, "Hrakleio");
    flights[1].price = 100.58;
    r = (rand() % 5)+1;
    flights[1].available_seats = r;

    flights[2].flight_id = 2;
    strcpy(flights[2].from_city, "Larissa");
    strcpy(flights[2].to_city, "Axladoxwri");
    flights[2].price = 70.56;
    r = (rand() % 5)+1;    
    flights[2].available_seats = r;
}

void seats (int fd[2], Pthseis flights[3], int* k, int Ep[3], float *total){ //diathesimes theseis

    read(fd[0], &flights[*k].available_seats, SIZE_OF_INT);  //diavazei tis diathesimes theseis kai tis pernaei mesa sth sunarthsh

    if (flights[*k].available_seats > 0){ //an uparxoun diathesimes pthseis mpainei eis to if 
        
        printf("Yparxoyn %d diathesimes theseis gia thn pthsh %d\n", flights[*k].available_seats, flights[*k].flight_id);
        flights[*k].available_seats -- ; //meiwsh theshs logw krathshs
        printf(" Kostos pthshs : %.2f \n", flights[*k].price);
        *total = (*total + flights[*k].price); // sunolika esoda aeroporikhs etairias
        Ep[*k] = Ep[*k] + 1; // pinakas me epituxeis krathseis

        write(fd[1], &flights[*k].available_seats, SIZE_OF_INT); //grafei tis diathesimes theseis kai tis epistrefei sth sunarthsh dhladh sth main

    }
    else{   //an den uparxoun theseis stop to loop kai gurna ston parent

        printf("Den uparxoun diathesimes theseis gia thn pthsh %d \n", flights[*k].flight_id);
        write(fd[1], &flights[*k].available_seats, SIZE_OF_INT);// to idio me panw
    }
}