#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Pokedex.h"
#include"Entrenador.h"
#include"Torneo.h"
#include"Menu.h"

int main(){
    Trainer *trainers[MAXTRAIN];
    List *pkdex;
    Torneo *tour;
    FILE *f;
    const char *trainerFile;
    int count, i;

    srand((unsigned)time(NULL));

    count = 0;
    for(i = 0; i < MAXTRAIN; i++){
        trainers[i] = NULL;
    }

    f = fopen("Entradas/Pokedex.txt", "r");

    if(f != NULL){
        fclose(f);
        pkdex = RegistPokedex("Entradas/Pokedex.txt");
    }else{
        pkdex = RegistPokedex("../Entradas/Pokedex.txt");
    }

    tour = NewTorneo();

    if((pkdex != NULL) && (tour != NULL)){
        trainerFile = "Entradas/Entrenadores.txt";
        f = fopen(trainerFile, "r");

        if(f == NULL){
            trainerFile = "../Entradas/Entrenadores.txt";
            f = fopen(trainerFile, "r");
        }

        if(f != NULL){
            fclose(f);

            ReadTrainers(trainerFile, trainers, &count);

            for(i = 0; i < count; i++){
                GenerateTeam(pkdex, trainers[i], 300);
            }

            SetTournamentTrainers(tour, trainers, count);

            if(count == MAXTRAIN){
                GenerateGroups(tour);
                GenerateGroupMatches(tour);
            }
        }

        MenuRun(tour, pkdex, trainers, &count);
    }else{
        fprintf(stderr, "Error: no se pudo iniciar la Pokedex o el torneo.\n");
    }

    for(i = 0; i < count; i++){
        DestroyTrainer(trainers[i]);
    }

    DestroyTorneo(tour);
    DestroyList(pkdex);

    return 0;
}
