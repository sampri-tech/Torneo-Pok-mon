#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Torneo.h"
#include"Pokedex.h"
#include"Entrenador.h"
#include"Pokemon.h"
#include"LIST.h"
#include"Menu.h"

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

static Trainer *FindTrainerById(Trainer *trainers[], int count, int id){
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
    if((tour != NULL) && (trainers != NULL) && (count == MAXTRAIN) && (GetMatchBlueId(tour, 1) == 0)){
        SetTournamentTrainers(tour, trainers, count);
        GenerateGroups(tour);
        GenerateGroupMatches(tour);
        printf("Torneo iniciado: grupos y combates de la fase de grupos generados.\n");
    }
}

static void ListTrainers(Trainer *trainers[], int count){
    int i;

    if((trainers != NULL) && (count > 0)){
        for(i = 0; i < count; i++){
            if(trainers[i] != NULL){
                printf("%d. ID: %d | %s\n", i + 1, GetTrainerId(trainers[i]), GetTrainerName(trainers[i]));
            }
        }
    }else{
        printf("No hay entrenadores registrados.\n");
    }
}

static void ShowTrainerInfo(const Trainer *train){
    if(train != NULL){
        printf("\nInformacion General del Entrenador\n");
        printf("ID: %d\n", GetTrainerId(train));
        printf("Nombre: %s\n", GetTrainerName(train));
        printf("Tipos favoritos: %s / %s\n", GetFirstPrefType(train), GetSecPrefType(train));
        printf("Puntos: %d\n", GetPoints(train));
        printf("Victorias: %d\n", GetWins(train));
        printf("Empates: %d\n", GetTies(train));
        printf("Derrotas: %d\n", GetDefeats(train));
        printf("Pokemon derrotados: %d\n", GetPkmDefeated(train));
        printf("Tamano del equipo: %d\n", GetTeamSize(train));
    }
}

static void ShowTeam(const Trainer *train){
    const Specimens *pkm;
    int i, size;

    if(train != NULL){
        size = GetTeamSize(train);
        printf("\nEquipo actual (%d/6)\n", size);

        if(size == 0){
            printf("Sin equipo.\n");
        }else{
            for(i = 0; i < size; i++){
                pkm = GetTeamMember(train, i);

                if(pkm != NULL){
                    printf("%d. %s | Nv.%d | HP %.0f/%.0f | Atk %d | Def %d | Vel %d\n", i + 1, GetPkmName(pkm), GetLevel(pkm),
                            GetActualHp(pkm), GetMaxHp(pkm), GetAtk(pkm), GetDef(pkm), GetSpd(pkm));
                }
            }
        }
    }
}

static const Species *SearchSpeciesInput(const List *pkdex, const char *input){
    const Species *sp;
    int numb;

    sp = NULL;

    if((pkdex != NULL) && (input != NULL) && (input[0] != '\0')){
        if(sscanf(input, "%d", &numb) == 1){
            sp = SearchNDex(pkdex, numb);
        }else{
            sp = SearchSpecies(pkdex, input);
        }
    }

    return sp;
}

static void MenuPokedex(const List *pkdex){
    const Species *sp;
    char name[20];
    int option, numb;

    option = -1;

    while(option != 0){
        printf("\nPokedex\n");
        PrintPokedex(pkdex);
        printf("\n1. Buscar por N. de Pokedex\n");
        printf("2. Buscar por Nombre\n");
        printf("0. Volver\n");
        option = ReadInt("Seleccione una opcion: ");

        if(option == 1){
            numb = ReadInt("N. de Pokedex: ");
            sp = SearchNDex(pkdex, numb);

            if(sp != NULL){
                PrintSpecie(sp);
            }else{
                printf("Especie no encontrada.\n");
            }
        }else if(option == 2){
            ReadString("Nombre del Pokemon: ", name, sizeof(name));
            sp = SearchSpecies(pkdex, name);

            if(sp != NULL){
                PrintSpecie(sp);
            }else{
                printf("Especie no encontrada.\n");
            }
        }
    }
}

static void MenuAddMember(const List *pkdex, Trainer *train){
    const Species *sp;
    char input[20];
    int maxLvl;

    if((pkdex != NULL) && (train != NULL)){
        if(GetTeamSize(train) < 6){
            printf("\nDespliegue de Pokedex\n");
            PrintPokedex(pkdex);
            ReadString("N. o Nombre de Pokedex (0 para volver): ", input, sizeof(input));

            if((input[0] != '\0') && (strcmp(input, "0") != 0)){
                sp = SearchSpeciesInput(pkdex, input);

                if(sp != NULL){
                    maxLvl = ReadInt("Nivel total del equipo: ");

                    if(maxLvl < (GetTeamSize(train) + 1)){
                        printf("Nivel total invalido: debe ser al menos %d.\n", GetTeamSize(train) + 1);
                    }else if(AddTeamMemb(train, sp, maxLvl)){
                        printf("Pokemon anadido al equipo.\n");
                    }else{
                        printf("No se pudo anadir el Pokemon.\n");
                    }
                }else{
                    printf("Especie no encontrada.\n");
                }
            }
        }else{
            printf("El equipo ya tiene 6 Pokemon.\n");
        }
    }
}

static void MenuDeleteMember(Trainer *train){
    int pos, maxLvl;

    if(train != NULL){
        if(GetTeamSize(train) >= 1){
            ShowTeam(train);
            pos = ReadInt("N. de miembro a eliminar (0 para volver): ");

            if(pos > 0){
                maxLvl = ReadInt("Nivel total del equipo: ");

                if(maxLvl < (GetTeamSize(train) - 1)){
                    printf("Nivel total invalido: debe ser al menos %d.\n", GetTeamSize(train) - 1);
                }else if(DelTeamMemb(train, pos - 1, maxLvl)){
                    printf("Pokemon eliminado del equipo.\n");
                }else{
                    printf("No se pudo eliminar el Pokemon.\n");
                }
            }
        }else{
            printf("El equipo no tiene miembros.\n");
        }
    }
}

static void MenuTeam(const Torneo *tour, const List *pkdex, Trainer *train){
    int option, started;

    option = -1;
    started = 0;

    if((tour != NULL) && (GetMatchBlueId(tour, 1) != 0)){
        started = 1;
    }

    if((pkdex != NULL) && (train != NULL)){
        while(option != 0){
            ShowTeam(train);
            printf("\n1. Hacer nuevo equipo\n");
            printf("2. Anadir miembro\n");
            printf("3. Borrar miembro\n");
            printf("4. Borrar equipo\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                if(started == 1){
                    printf("No se puede modificar el equipo: el torneo ya ha iniciado.\n");
                }else if(GetTeamSize(train) == 0){
                    int maxLvl;

                    maxLvl = ReadInt("Nivel total del equipo: ");

                    if(maxLvl < 6){
                        printf("Nivel total invalido: debe ser al menos 6.\n");
                    }else if(GenerateTeam(pkdex, train, maxLvl)){
                        printf("Equipo generado.\n");
                        ShowTeam(train);
                    }else{
                        printf("No se pudo generar el equipo.\n");
                    }
                }else{
                    printf("Solo se puede hacer un nuevo equipo si no tiene equipo.\n");
                }
            }else if(option == 2){
                if(started == 1){
                    printf("No se puede modificar el equipo: el torneo ya ha iniciado.\n");
                }else if(GetTeamSize(train) < 6){
                    MenuAddMember(pkdex, train);
                }else{
                    printf("El equipo ya tiene 6 Pokemon.\n");
                }
            }else if(option == 3){
                if(started == 1){
                    printf("No se puede modificar el equipo: el torneo ya ha iniciado.\n");
                }else if(GetTeamSize(train) >= 1){
                    MenuDeleteMember(train);
                }else{
                    printf("El equipo no tiene miembros.\n");
                }
            }else if(option == 4){
                if(started == 1){
                    printf("No se puede modificar el equipo: el torneo ya ha iniciado.\n");
                }else if(GetTeamSize(train) > 0){
                    ClearTeam(train);
                    printf("Equipo borrado.\n");
                }else{
                    printf("No existe un equipo para borrar.\n");
                }
            }
        }
    }
}

static void MenuTrainerInfo(const Torneo *tour, const List *pkdex, Trainer *train){
    int option;

    option = -1;

    if((tour != NULL) && (pkdex != NULL) && (train != NULL)){
        while(option != 0){
            ShowTrainerInfo(train);
            printf("\n1. Mostrar Equipo\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                MenuTeam(tour, pkdex, train);
            }
        }
    }
}

static void MenuAddTrainer(Torneo *tour, Trainer *trainers[], int *count){
    char name[20], type1[20], type2[20];

    if((trainers != NULL) && (count != NULL)){
        ReadString("Nombre: ", name, sizeof(name));
        ReadString("Tipo 1 (- si no tiene): ", type1, sizeof(type1));
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

static void MenuDeleteTrainer(Trainer *trainers[], int *count){
    Trainer *train;
    int id, i, pos, found;

    if((trainers != NULL) && (count != NULL)){
        id = ReadInt("N. de Entrenador (ID) a eliminar (0 para volver): ");

        if(id > 0){
            train = NULL;
            pos = -1;
            i = 0;
            found = 0;

            while((i < *count) && (found == 0)){
                if((trainers[i] != NULL) && (GetTrainerId(trainers[i]) == id)){
                    train = trainers[i];
                    pos = i;
                    found = 1;
                }
                i++;
            }

            if((train != NULL) && (pos >= 0)){
                DestroyTrainer(train);

                for(i = pos; i < (*count - 1); i++){
                    trainers[i] = trainers[i + 1];
                }

                trainers[*count - 1] = NULL;
                (*count)--;
                printf("Entrenador eliminado.\n");
            }else{
                printf("Entrenador no encontrado.\n");
            }
        }
    }
}

static void MenuTrainers(Torneo *tour, const List *pkdex, Trainer *trainers[], int *count){
    Trainer *train;
    int option, id;

    option = -1;

    if((tour != NULL) && (pkdex != NULL) && (trainers != NULL) && (count != NULL)){
        while(option != 0){
            printf("\nEntrenadores\n");
            ListTrainers(trainers, *count);
            printf("\n1. Seleccionar Entrenador\n");
            printf("2. Anadir Entrenador\n");
            printf("3. Eliminar Entrenador\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                id = ReadInt("N. de Entrenador (ID): ");
                train = FindTrainerById(trainers, *count, id);

                if(train != NULL){
                    MenuTrainerInfo(tour, pkdex, train);
                }else{
                    printf("Entrenador no encontrado.\n");
                }
            }else if(option == 2){
                if(GetMatchBlueId(tour, 1) != 0){
                    printf("No se puede anadir entrenadores: el torneo ya ha iniciado.\n");
                }else{
                    MenuAddTrainer(tour, trainers, count);
                }
            }else if(option == 3){
                if(GetMatchBlueId(tour, 1) != 0){
                    printf("No se puede eliminar entrenadores: el torneo ya ha iniciado.\n");
                }else{
                    MenuDeleteTrainer(trainers, count);
                }
            }
        }
    }
}

static void MenuLoadResults(Torneo *tour, Trainer *trainers[], int count){
    char file[128];
    int option, numbCombat, blueId, redId, winnerId;

    option = -1;

    if((tour != NULL) && (trainers != NULL)){
        StartTournamentIfReady(tour, trainers, count);

        while(option != 0){
            printf("\nCargar Resultados\n");
            printf("1. Cargar desde archivo\n");
            printf("2. Cargar por teclado\n");
            printf("3. Guardar resultados\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                ReadString("Archivo de resultados/torneo guardado (0 para volver): ", file, sizeof(file));

                if((file[0] != '\0') && (strcmp(file, "0") != 0)){
                    if(GetMatchBlueId(tour, 1) != 0){
                        LoadResultsFromFile(tour, file);
                    }else{
                        printf("El torneo aun no esta listo. Se requieren %d entrenadores con grupos generados.\n", MAXTRAIN);
                    }
                }
            }else if(option == 2){
                if(GetMatchBlueId(tour, 1) != 0){
                    numbCombat = ReadInt("Numero de combate (0 para volver): ");

                    if(numbCombat != 0){
                        blueId = GetMatchBlueId(tour, numbCombat);
                        redId = GetMatchRedId(tour, numbCombat);

                        if((numbCombat < 1) || (numbCombat > TORNEO_TOTAL_COMBATS)){
                            printf("Combate inexistente.\n");
                        }else if((blueId == 0) || (redId == 0)){
                            printf("El combate no existe o aun no tiene participantes.\n");
                        }else if(IsMatchPlayed(tour, numbCombat) == 1){
                            printf("El combate %d ya tiene resultado.\n", numbCombat);
                        }else{
                            printf("Combate %d: %d vs %d\n", numbCombat, blueId, redId);
                            winnerId = ReadInt("ID del ganador (0 = empate, solo fase de grupos): ");
                            SetMatchResult(tour, numbCombat, winnerId);
                        }
                    }
                }else{
                    printf("El torneo aun no esta listo. Se requieren %d entrenadores con grupos generados.\n", MAXTRAIN);
                }
            }else if(option == 3){
                ReadString("Archivo para guardar resultados (0 para volver): ", file, sizeof(file));

                if((file[0] != '\0') && (strcmp(file, "0") != 0)){
                    SaveResultsToFile(tour, file);
                }
            }
        }
    }
}

static void MenuTournament(Torneo *tour, Trainer *trainers[], int count){
    Trainer *blue, *red;
    int option, i, numbCombat, blueId, redId, winnerId, pending;

    option = -1;

    if((tour != NULL) && (trainers != NULL)){
        StartTournamentIfReady(tour, trainers, count);

        while(option != 0){
            printf("\nTorneo\n");
            printf("1. Simular combates pendientes\n");
            printf("2. Simular combate especifico\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                if(GetMatchBlueId(tour, 1) != 0){
                    pending = 0;

                    for(i = 1; i <= TORNEO_TOTAL_COMBATS; i++){
                        if((GetMatchBlueId(tour, i) != 0) && (GetMatchRedId(tour, i) != 0) && (IsMatchPlayed(tour, i) == 0)){
                            pending++;
                        }
                    }

                    if(pending == 0){
                        printf("No hay combates pendientes para simular.\n");
                    }else{
                        for(i = 1; i <= TORNEO_TOTAL_COMBATS; i++){
                            blueId = GetMatchBlueId(tour, i);
                            redId = GetMatchRedId(tour, i);

                            if((blueId != 0) && (redId != 0) && (IsMatchPlayed(tour, i) == 0)){
                                blue = FindTrainerById(trainers, count, blueId);
                                red = FindTrainerById(trainers, count, redId);

                                if((blue != NULL) && (red != NULL) && (GetTeamSize(blue) > 0) && (GetTeamSize(red) > 0)){
                                    printf("\nCombate %d: %s vs %s\n", i, GetTrainerName(blue), GetTrainerName(red));
                                    winnerId = LiveCombat(blue, red, i);
                                    SetLiveMatchResult(tour, i, winnerId);

                                    if(winnerId == 0){
                                        printf("Resultado: Empate\n");
                                    }else if(winnerId == blueId){
                                        printf("Ganador: %s\n", GetTrainerName(blue));
                                    }else{
                                        printf("Ganador: %s\n", GetTrainerName(red));
                                    }
                                }else{
                                    printf("No se pudo simular el combate %d.\n", i);
                                }
                            }
                        }
                    }
                }else{
                    printf("El torneo aun no esta listo. Se requieren %d entrenadores.\n", MAXTRAIN);
                }
            }else if(option == 2){
                if(GetMatchBlueId(tour, 1) != 0){
                    numbCombat = ReadInt("Numero de combate (0 para volver): ");

                    if(numbCombat != 0){
                        if((numbCombat < 1) || (numbCombat > TORNEO_TOTAL_COMBATS)){
                            printf("Combate inexistente.\n");
                        }else{
                            blueId = GetMatchBlueId(tour, numbCombat);
                            redId = GetMatchRedId(tour, numbCombat);

                            if((blueId == 0) || (redId == 0)){
                                printf("El combate no existe o aun no tiene participantes.\n");
                            }else if(IsMatchPlayed(tour, numbCombat) == 1){
                                printf("El combate %d ya tiene resultado.\n", numbCombat);
                            }else{
                                blue = FindTrainerById(trainers, count, blueId);
                                red = FindTrainerById(trainers, count, redId);

                                if((blue == NULL) || (red == NULL)){
                                    printf("No se encontro alguno de los entrenadores.\n");
                                }else if((GetTeamSize(blue) == 0) || (GetTeamSize(red) == 0)){
                                    printf("Ambos entrenadores deben tener un equipo para combatir.\n");
                                }else{
                                    printf("\nCombate %d: %s vs %s\n", numbCombat, GetTrainerName(blue), GetTrainerName(red));
                                    winnerId = LiveCombat(blue, red, numbCombat);
                                    SetLiveMatchResult(tour, numbCombat, winnerId);

                                    if(winnerId == 0){
                                        printf("Resultado: Empate\n");
                                    }else if(winnerId == blueId){
                                        printf("Ganador: %s\n", GetTrainerName(blue));
                                    }else{
                                        printf("Ganador: %s\n", GetTrainerName(red));
                                    }
                                }
                            }
                        }
                    }
                }else{
                    printf("El torneo aun no esta listo. Se requieren %d entrenadores.\n", MAXTRAIN);
                }
            }
        }
    }
}

static void MenuShowChampion(const Torneo *tour, Trainer *trainers[], int count){
    Trainer *train;
    int id;

    if((tour != NULL) && (trainers != NULL)){
        id = GetChampionId(tour);
        train = FindTrainerById(trainers, count, id);

        if(train != NULL){
            printf("\nCampeon\n");
            ShowTrainerInfo(train);
            ShowTeam(train);
        }else{
            printf("Campeon: pendiente\n");
        }
    }
}

static void MenuShowPodium(const Torneo *tour, Trainer *trainers[], int count){
    Trainer *champ, *runner, *third, *fourth;
    int champId, runnerId, thirdId, fourthId;

    if((tour != NULL) && (trainers != NULL)){
        champId = GetChampionId(tour);
        runnerId = GetMatchWinnerId(tour, 64);

        if(runnerId == GetMatchBlueId(tour, 64)){
            runnerId = GetMatchRedId(tour, 64);
        }else if(runnerId == GetMatchRedId(tour, 64)){
            runnerId = GetMatchBlueId(tour, 64);
        }else{
            runnerId = 0;
        }

        thirdId = GetMatchWinnerId(tour, 63);
        fourthId = 0;

        if(thirdId == GetMatchBlueId(tour, 63)){
            fourthId = GetMatchRedId(tour, 63);
        }else if(thirdId == GetMatchRedId(tour, 63)){
            fourthId = GetMatchBlueId(tour, 63);
        }else{
            thirdId = 0;
        }

        champ = FindTrainerById(trainers, count, champId);
        runner = FindTrainerById(trainers, count, runnerId);
        third = FindTrainerById(trainers, count, thirdId);
        fourth = FindTrainerById(trainers, count, fourthId);

        printf("\nPodios (1., 2., 3. y 4. lugar)\n");

        if(champ != NULL){
            printf("\n1. lugar\n");
            ShowTrainerInfo(champ);
        }else{
            printf("\n1. lugar: pendiente\n");
        }

        if(runner != NULL){
            printf("\n2. lugar\n");
            ShowTrainerInfo(runner);
        }else{
            printf("\n2. lugar: pendiente\n");
        }

        if(third != NULL){
            printf("\n3. lugar\n");
            ShowTrainerInfo(third);
        }else{
            printf("\n3. lugar: pendiente\n");
        }

        if(fourth != NULL){
            printf("\n4. lugar\n");
            ShowTrainerInfo(fourth);
        }else{
            printf("\n4. lugar: pendiente\n");
        }
    }
}

static void PrintMatchDetail(const Torneo *tour, Trainer *trainers[], int count, int numbCombat){
    Trainer *blue, *red, *win;
    int blueId, redId, winnerId;

    if((tour != NULL) && (trainers != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        blueId = GetMatchBlueId(tour, numbCombat);
        redId = GetMatchRedId(tour, numbCombat);
        winnerId = GetMatchWinnerId(tour, numbCombat);
        blue = FindTrainerById(trainers, count, blueId);
        red = FindTrainerById(trainers, count, redId);
        win = FindTrainerById(trainers, count, winnerId);

        printf("\nDetalle del combate %d\n", numbCombat);

        if((blue != NULL) && (red != NULL)){
            printf("%s vs %s\n", GetTrainerName(blue), GetTrainerName(red));

            if(IsMatchPlayed(tour, numbCombat) == 1){
                if(winnerId == 0){
                    printf("Resultado: Empate\n");
                }else if(win != NULL){
                    printf("Ganador: %s\n", GetTrainerName(win));
                }
            }else{
                printf("Resultado: Pendiente\n");
            }

            printf("\nEntrenador 1\n");
            ShowTrainerInfo(blue);
            printf("\nEntrenador 2\n");
            ShowTrainerInfo(red);
        }else{
            printf("Combate no disponible.\n");
        }
    }
}

static void PrintRangeMatches(const Torneo *tour, int start, int end){
    int i;

    if(tour != NULL){
        for(i = start; i <= end; i++){
            if(GetMatchBlueId(tour, i) != 0){
                PrintMatch(tour, i);
            }
        }
    }
}

static void MenuClassificatorias(const Torneo *tour, Trainer *trainers[], int count){
    int option, numbCombat;

    option = -1;

    if((tour != NULL) && (trainers != NULL)){
        while(option != 0){
            printf("\nClasificatorias\n");
            printf("1. Tabla Clasificatoria\n");
            printf("2. Combates\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                PrintStandings(tour);
            }else if(option == 2){
                numbCombat = -1;

                while(numbCombat != 0){
                    printf("\nCombates de Clasificatorias\n");
                    PrintRangeMatches(tour, 1, TORNEO_GROUP_COMBATS);
                    numbCombat = ReadInt("N. de Combate para detalle (0 para volver): ");

                    if((numbCombat >= 1) && (numbCombat <= TORNEO_GROUP_COMBATS)){
                        PrintMatchDetail(tour, trainers, count, numbCombat);
                    }
                }
            }
        }
    }
}

static void MenuEliminatorias(const Torneo *tour, Trainer *trainers[], int count){
    int option, numbCombat;

    option = -1;

    if((tour != NULL) && (trainers != NULL)){
        while(option != 0){
            printf("\nEliminatorias\n");
            printf("1. Tabla / Llaves Eliminatorias\n");
            printf("2. Combates\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                PrintRangeMatches(tour, TORNEO_GROUP_COMBATS + 1, TORNEO_TOTAL_COMBATS);
            }else if(option == 2){
                numbCombat = -1;

                while(numbCombat != 0){
                    printf("\nCombates de Eliminatorias\n");
                    PrintRangeMatches(tour, TORNEO_GROUP_COMBATS + 1, TORNEO_TOTAL_COMBATS);
                    numbCombat = ReadInt("N. de Combate para detalle (0 para volver): ");

                    if((numbCombat > TORNEO_GROUP_COMBATS) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
                        PrintMatchDetail(tour, trainers, count, numbCombat);
                    }
                }
            }
        }
    }
}

static void MenuResults(Torneo *tour, Trainer *trainers[], int count){
    int option;

    option = -1;

    if((tour != NULL) && (trainers != NULL)){
        while(option != 0){
            printf("\nResultados\n");
            printf("1. Campeon\n");
            printf("2. Podios (1., 2., 3. y 4. lugar)\n");
            printf("3. Clasificatorias\n");
            printf("4. Eliminatorias\n");
            printf("0. Volver\n");
            option = ReadInt("Seleccione una opcion: ");

            if(option == 1){
                MenuShowChampion(tour, trainers, count);
            }else if(option == 2){
                MenuShowPodium(tour, trainers, count);
            }else if(option == 3){
                MenuClassificatorias(tour, trainers, count);
            }else if(option == 4){
                MenuEliminatorias(tour, trainers, count);
            }
        }
    }
}

static void ShowMainMenu(void){
    printf("\n========================================\n");
    printf("          GRAN TORNEO POKEMON\n");
    printf("========================================\n");
    printf("1. Pokedex\n");
    printf("2. Entrenadores\n");
    printf("3. Cargar Resultados\n");
    printf("4. Torneo\n");
    printf("5. Resultados\n");
    printf("6. Salir\n");
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
                    MenuTrainers(tour, pkdex, trainers, count);
                    break;
                case 3:
                    MenuLoadResults(tour, trainers, *count);
                    break;
                case 4:
                    MenuTournament(tour, trainers, *count);
                    break;
                case 5:
                    MenuResults(tour, trainers, *count);
                    break;
                case 6:
                    SaveResultsToFile(tour, "resultados.txt");
                    SaveStandingsToFile(tour, "clasificacion.txt");
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
