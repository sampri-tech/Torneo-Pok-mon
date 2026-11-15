#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Torneo.h"
#include"Pokedex.h"
#include"Entrenador.h"
#include"Pokemon.h"
#include"LIST.h"
#include"Menu.h"

static void ShowMainMenu(void){
    printf("\n========================================\n");
    printf("          GRAN TORNEO POKEMON\n");
    printf("========================================\n");
    printf("1. Consultar Pokedex\n");
    printf("2. Registrar entrenador\n");
    printf("3. Crear equipo\n");
    printf("4. Consultar entrenadores\n");
    printf("5. Consultar equipos\n");
    printf("6. Resultados y combates\n");
    printf("7. Consultar clasificacion\n");
    printf("8. Consultar enfrentamientos\n");
    printf("9. Consultar historial de combates\n");
    printf("10. Mostrar resultados del torneo\n");
    printf("11. Mostrar campeon\n");
    printf("12. Salir\n");
}

static int ReadInt(const char *msg){
    char line[32];
    int value;

    value = 0;

    if(msg != NULL){
        printf("%s", msg);
    }

    if(fgets(line, sizeof(line), stdin) != NULL){
        if(sscanf(line, "%d", &value) != 1){
            value = 0;
        }
    }

    return value;
}

static void ReadString(const char *msg, char *dest, size_t size){
    if((msg != NULL) && (dest != NULL) && (size > 0)){
        printf("%s", msg);

        if(fgets(dest, (int)size, stdin) != NULL){
            dest[strcspn(dest, "\n")] = '\0';
        }
    }
}

static Trainer *FindTrainer(Trainer *trainers[], int count, int id){
    Trainer *train;
    int i;

    train = NULL;
    i = 0;

    if((trainers != NULL) && (count >= 0) && (id > 0)){
        while((i < count) && (train == NULL)){
            if((trainers[i] != NULL) && (GetTrainerId(trainers[i]) == id)){
                train = trainers[i];
            }
            i++;
        }
    }

    return train;
}

static void StartTournamentIfReady(Torneo *tour, Trainer *trainers[], int count){
    if((tour != NULL) && (trainers != NULL) && (count == MAXTRAIN) &&
       (GetMatchBlueId(tour, 1) == 0)){
        SetTournamentTrainers(tour, trainers, count);
        GenerateGroups(tour);
        GenerateGroupMatches(tour);
        printf("Torneo iniciado: grupos y combates de la fase de grupos generados.\n");
    }
}

static void MenuPokedex(const List *pkdex){
    const Species *sp;
    char name[20];
    int option, numb;

    option = 1;

    while(option != 0){
        printf("\nPokedex\n");
        printf("1. Mostrar Pokedex\n");
        printf("2. Buscar por numero\n");
        printf("3. Buscar por nombre\n");
        printf("0. Volver\n");
        option = ReadInt("Seleccione una opcion: ");

        if(option == 1){
            PrintPokedex(pkdex);
        }else if(option == 2){
            numb = ReadInt("Numero: ");
            sp = SearchNDex(pkdex, numb);
            PrintSpecie(sp);
        }else if(option == 3){
            ReadString("Nombre: ", name, sizeof(name));
            sp = SearchSpecies(pkdex, name);
            PrintSpecie(sp);
        }
    }
}

static void MenuRegisterTrainer(Torneo *tour, Trainer *trainers[], int *count){
    char name[20], type1[20], type2[20];

    if((trainers != NULL) && (count != NULL)){
        ReadString("Nombre: ", name, sizeof(name));
        ReadString("Tipo 1: ", type1, sizeof(type1));
        ReadString("Tipo 2 (- si no tiene): ", type2, sizeof(type2));

        if(name[0] == '\0'){
            printf("Nombre invalido.\n");
        }else{
            if(type1[0] == '\0'){
                strcpy(type1, "-");
            }

            if(type2[0] == '\0'){
                strcpy(type2, "-");
            }

            if(AddTrainer(trainers, count, name, type1, type2)){
                printf("Entrenador registrado.\n");
                StartTournamentIfReady(tour, trainers, *count);
            }else{
                printf("No se pudo registrar el entrenador.\n");
            }
        }
    }
}

static void MenuCreateTeam(const List *pkdex, Trainer *trainers[], int count){
    Trainer *train;
    int id, lvl;

    if((pkdex != NULL) && (trainers != NULL)){
        id = ReadInt("ID del entrenador: ");
        train = FindTrainer(trainers, count, id);

        if(train != NULL){
            lvl = ReadInt("Nivel de los Pokemon: ");

            if(GenerateTeam(pkdex, train, lvl)){
                printf("Equipo generado.\n");
            }else{
                printf("No se pudo generar el equipo.\n");
            }
        }else{
            printf("Entrenador no encontrado.\n");
        }
    }
}

static void MenuConsultTrainers(Trainer *trainers[], int count){
    int i;

    if((trainers != NULL) && (count > 0)){
        for(i = 0; i < count; i++){
            if(trainers[i] != NULL){
                printf("%d %s | Pts: %d | V: %d | E: %d | D: %d | Equipo: %d\n",
                       GetTrainerId(trainers[i]),
                       GetTrainerName(trainers[i]),
                       GetPoints(trainers[i]),
                       GetWins(trainers[i]),
                       GetTies(trainers[i]),
                       GetDefeats(trainers[i]),
                       GetTeamSize(trainers[i]));
            }
        }
    }else{
        printf("No hay entrenadores registrados.\n");
    }
}

static void MenuConsultTeams(Trainer *trainers[], int count){
    const Trainer *train;
    const Specimens *pkm;
    int i, j, size;

    if((trainers != NULL) && (count > 0)){
        for(i = 0; i < count; i++){
            train = trainers[i];

            if(train != NULL){
                printf("%d %s\n", GetTrainerId(train), GetTrainerName(train));
                size = GetTeamSize(train);

                if(size == 0){
                    printf("  Sin equipo.\n");
                }else{
                    for(j = 0; j < size; j++){
                        pkm = GetTeamMember(train, j);

                        if(pkm != NULL){
                            printf("  %s Nv.%d HP %.0f/%.0f\n",
                                   GetPkmName(pkm),
                                   GetLevel(pkm),
                                   GetActualHp(pkm),
                                   GetMaxHp(pkm));
                        }
                    }
                }
            }
        }
    }else{
        printf("No hay entrenadores registrados.\n");
    }
}

static void MenuLoadResults(Torneo *tour, Trainer *trainers[], int count){
    char file[128];
    Trainer *blue, *red;
    int option, numbCombat, blueId, redId, winnerId;

    StartTournamentIfReady(tour, trainers, count);

    if((tour != NULL) && (GetMatchBlueId(tour, 1) != 0)){
        option = 1;

        while(option != 0){
            printf("\nResultados y combates\n");
            printf("1. Cargar desde archivo\n");
            printf("2. Cargar por teclado\n");
            printf("3. Simular combate\n");
            printf("4. Guardar resultados\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                ReadString("Archivo de resultados: ", file, sizeof(file));
                LoadResultsFromFile(tour, file);
            }else if(option == 2){
                numbCombat = ReadInt("Numero de combate: ");
                blueId = GetMatchBlueId(tour, numbCombat);
                redId = GetMatchRedId(tour, numbCombat);

                if((blueId == 0) || (redId == 0)){
                    printf("El combate no existe o aun no tiene participantes.\n");
                }else if(IsMatchPlayed(tour, numbCombat) == 1){
                    printf("El combate %d ya tiene resultado.\n", numbCombat);
                }else{
                    printf("Combate %d: %d vs %d\n", numbCombat, blueId, redId);
                    winnerId = ReadInt("ID del ganador (0 = empate): ");
                    SetMatchResult(tour, numbCombat, winnerId);
                }
            }else if(option == 3){
                numbCombat = ReadInt("Numero de combate: ");
                blueId = GetMatchBlueId(tour, numbCombat);
                redId = GetMatchRedId(tour, numbCombat);

                if((blueId == 0) || (redId == 0)){
                    printf("El combate no existe o aun no tiene participantes.\n");
                }else if(IsMatchPlayed(tour, numbCombat) == 1){
                    printf("El combate %d ya tiene resultado.\n", numbCombat);
                }else{
                    blue = FindTrainer(trainers, count, blueId);
                    red = FindTrainer(trainers, count, redId);

                    if((blue == NULL) || (red == NULL)){
                        printf("No se encontro alguno de los entrenadores.\n");
                    }else if((GetTeamSize(blue) == 0) || (GetTeamSize(red) == 0)){
                        printf("Ambos entrenadores deben tener un equipo para combatir.\n");
                    }else{
                        printf("Combate %d: %s vs %s\n",
                               numbCombat, GetTrainerName(blue), GetTrainerName(red));
                        winnerId = LiveCombat(blue, red, numbCombat);
                        SetLiveMatchResult(tour, numbCombat, winnerId);

                        if(winnerId == 0){
                            printf("Resultado: empate.\n");
                        }else if(winnerId == blueId){
                            printf("Ganador: %s\n", GetTrainerName(blue));
                        }else{
                            printf("Ganador: %s\n", GetTrainerName(red));
                        }
                    }
                }
            }else if(option == 4){
                ReadString("Archivo de resultados: ", file, sizeof(file));
                SaveResultsToFile(tour, file);
            }
        }
    }else{
        printf("El torneo aun no esta listo. Se requieren %d entrenadores.\n", MAXTRAIN);
    }
}

static void MenuConsultStandings(const Torneo *tour){
    char file[128];
    int option;

    if(tour != NULL){
        option = 1;

        while(option != 0){
            printf("\nClasificacion\n");
            printf("1. Mostrar clasificacion\n");
            printf("2. Guardar clasificacion\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                PrintStandings(tour);
            }else if(option == 2){
                ReadString("Archivo de clasificacion: ", file, sizeof(file));
                SaveStandingsToFile(tour, file);
            }
        }
    }
}

static void MenuConsultMatches(const Torneo *tour){
    PrintMatches(tour);
}

static void MenuConsultHistory(const Torneo *tour){
    PrintHistory(tour);
}

static void MenuShowResults(const Torneo *tour){
    PrintFinalResults(tour);
}

static void MenuShowChampion(const Torneo *tour, Trainer *trainers[], int count){
    const Trainer *train;
    int id, i;

    train = NULL;
    i = 0;

    if((tour != NULL) && (trainers != NULL)){
        id = GetChampionId(tour);

        while((i < count) && (train == NULL)){
            if((trainers[i] != NULL) && (GetTrainerId(trainers[i]) == id)){
                train = trainers[i];
            }
            i++;
        }

        if(train != NULL){
            printf("Campeon: %s\n", GetTrainerName(train));
        }else{
            printf("Campeon: pendiente\n");
        }
    }
}

void MenuRun(Torneo *tour, const List *pkdex, Trainer *trainers[], int *count){
    int option, reading;

    if((tour != NULL) && (pkdex != NULL) && (trainers != NULL) && (count != NULL)){
        reading = 1;
        StartTournamentIfReady(tour, trainers, *count);

        while(reading == 1){
            ShowMainMenu();
            option = ReadInt("Seleccione una opcion: ");

            switch(option){
                case 1:
                    MenuPokedex(pkdex);
                    break;
                case 2:
                    MenuRegisterTrainer(tour, trainers, count);
                    break;
                case 3:
                    MenuCreateTeam(pkdex, trainers, *count);
                    break;
                case 4:
                    MenuConsultTrainers(trainers, *count);
                    break;
                case 5:
                    MenuConsultTeams(trainers, *count);
                    break;
                case 6:
                    MenuLoadResults(tour, trainers, *count);
                    break;
                case 7:
                    MenuConsultStandings(tour);
                    break;
                case 8:
                    MenuConsultMatches(tour);
                    break;
                case 9:
                    MenuConsultHistory(tour);
                    break;
                case 10:
                    MenuShowResults(tour);
                    break;
                case 11:
                    MenuShowChampion(tour, trainers, *count);
                    break;
                case 12:
                    reading = 0;
                    break;
                default:
                    printf("Opcion invalida.\n");
                    break;
            }
        }
    }else{
        printf("Error: el menu no tiene un contexto valido.\n");
    }
}
