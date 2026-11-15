#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Entrenador.h"
#include"Pokemon.h"
#include"Combate.h"


struct ResultCombat{
    int numbCombat; // Numero del combate
    int blueId; // ID del primer entrenador
    int redId; // ID del segundo entrenador
    int winnerId; // ID del ganador; 0 si hay empate
};

static int Random(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

static void FairGame(Trainer *train){
    int i;

    for(i = 0; i < GetTeamSize(train); i++){
        RestorePkmn(GetTeamMember(train, i));
    }
}

static Specimens *Fastest(Specimens *pkmBlue, Specimens *pkmRed){
    Specimens *fast;

    if((pkmBlue == NULL) || (pkmRed == NULL)) return NULL;

    if(GetSpd(pkmBlue) < GetSpd(pkmRed)){
        fast = pkmRed;
    }else{
        if(GetSpd(pkmBlue) == GetSpd(pkmRed)){
            if(Random(0, 1) == 0){
                fast = pkmBlue;
            }else{
                fast = pkmRed;
            }
        }else{
            fast = pkmBlue;
        }
    }

    return fast;
}

static int DefType(const char *ty){
    int i, sig;
    char types[18][20] ={"Acero", "Agua", "Bicho", "Dragón", "Eléctrico", "Fantasma",
    "Fuego", "Hada", "Hielo", "Lucha", "Normal", "Planta",
    "Psíquico", "Roca", "Siniestro", "Tierra", "Veneno", "Volador"};

    i = 0;
    sig = 1;
    if(ty != NULL){
        while(i < 18 && sig != 0){
            if(strcmp(ty, types[i]) == 0){
                sig = 0;
            }else{
                i++;
            }
        }
    }
    if(i >= 18 || ty == NULL) i = -1;

    return i;
}

static const float efect[18][18] = { { 0.5, 0.5, 1.0, 1.0, 0.5, 1.0, 0.5, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0 },
{ 1.0, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 2.0, 1.0, 2.0, 1.0, 1.0 },
{ 0.5, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 1.0, 0.5, 1.0, 2.0, 2.0, 1.0, 2.0, 1.0, 0.5, 0.5 },
{ 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
{ 1.0, 2.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 0.0, 1.0, 2.0 },
{ 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0 },
{ 2.0, 0.5, 2.0, 0.5, 1.0, 1.0, 0.5, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0 },
{ 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0 },
{ 0.5, 0.5, 1.0, 2.0, 1.0, 1.0, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 2.0 },
{ 2.0, 1.0, 0.5, 1.0, 1.0, 0.0, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 0.5, 2.0, 2.0, 1.0, 0.5, 0.5 },
{ 0.5, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0 },
{ 0.5, 2.0, 0.5, 0.5, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 2.0, 1.0, 2.0, 0.5, 0.5 },
{ 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 1.0, 0.0, 1.0, 2.0, 1.0 }, 
{ 0.5, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 2.0 },
{ 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0 },
{ 2.0, 1.0, 0.5, 1.0, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 2.0, 1.0, 1.0, 2.0, 0.0 }, 
{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5, 0.5, 1.0 }, 
{ 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0 } };

static float Effect(const char *atkType, const char *defType){
    int atk, def;
    float mult;

    mult = 1.0;
    atk = DefType(atkType);
    def = DefType(defType);

    if((atk >= 0) && (def >= 0)){
        mult = efect[atk][def];
    }

    return mult;
}

static void TurnAtk(Specimens *attacker, Specimens *defender, float *noDmg){
    float dmg;
    const char *atkType1, *atkType2, *defType1, *defType2;

    if((attacker != NULL) && (defender != NULL) && (noDmg != NULL)){
        atkType1 = GetSpecimenFirstType(attacker);
        atkType2 = GetSpecimenSecondType(attacker);
        defType1 = GetSpecimenFirstType(defender);
        defType2 = GetSpecimenSecondType(defender);

        dmg = 0;
        if((atkType1 != NULL) && (defType1 != NULL)){
            dmg = ((((2 * GetLevel(attacker)) / 5) + 2) * Random(40, 250) * ((float)GetAtk(attacker) / GetDef(defender)) / 50) + 2;

            dmg *= Effect(atkType1, defType1);
            if(defType2 != NULL){
                dmg *= Effect(atkType1, defType2);
            }

            if(atkType2 != NULL){
                dmg *= Effect(atkType2, defType1);
                if(defType2 != NULL){
                    dmg *= Effect(atkType2, defType2);
                }
            }
        }

        if(dmg == 0){
            *noDmg += 0.5;

            if(*noDmg >= 1){
                dmg = ((((2 * GetLevel(attacker)) / 5) + 2) * ((float)GetAtk(attacker) / GetDef(defender))) + 2;

                UpdateHp(defender, dmg);
                UpdateHp(attacker, dmg / 2);
            }
        }else{
            *noDmg = 0;
            UpdateHp(defender, dmg);
        }
    }
}

static int CountFainted(const Trainer *train){
    int i, count;

    count = 0;

    for(i = 0; i < GetTeamSize(train); i++){
        if(IsFainted(GetTeamMember(train, i))){
            count++;
        }
    }

    return count;
}

static int ApplyResult(Trainer *blue, Trainer *red, int numbCombat){
    Trainer *winner, *loser;
    int bFaint, rFaint, winId;

    bFaint = IsFainted(GetTeamMember(blue, GetTeamSize(blue) - 1));
    rFaint = IsFainted(GetTeamMember(red, GetTeamSize(red) - 1));

    winner = NULL;
    loser = NULL;

    if(bFaint && rFaint){
        if(numbCombat > 48){
            if(GetPoints(red) != GetPoints(blue)){
                winner = (GetPoints(red) > GetPoints(blue)) ? red : blue;
            }else if(GetPkmDefeated(red) != GetPkmDefeated(blue)){
                winner = (GetPkmDefeated(red) > GetPkmDefeated(blue)) ? red : blue;
            }else{
                winner = (Random(0, 1) == 0) ? blue : red;
            }
            loser = (winner == blue) ? red : blue;
        }
    }else if(bFaint){
        winner = red;
        loser = blue;
    }else{
        winner = blue;
        loser = red;
    }

    if(winner != NULL){
        winId = GetTrainerId(winner);
        AddWins(winner);
        AddDefeats(loser);
        AddPoints(winner, 3);
    }else{
        winId = 0;
        AddTies(blue);
        AddTies(red);
        AddPoints(blue, 1);
        AddPoints(red, 1);
    }

    if(numbCombat <= 48){
        AddPkmDefeated(blue, CountFainted(red));
        AddPkmDefeated(red, CountFainted(blue));
    }

    return winId;
}

static void InnerResult(List *Combats, Trainer *blue, Trainer *red){
    Result res;
    int numbCombat, winId;

    numbCombat = GetLSize(Combats) + 1;
    winId = ApplyResult(blue, red, numbCombat);

    res.numbCombat = numbCombat;
    res.blueId = GetTrainerId(blue);
    res.redId = GetTrainerId(red);
    res.winnerId = winId;

    Add(Combats, &res, sizeof(Result), GetLSize(Combats));
}


static void Simulate(Trainer *blue, Trainer *red){
    int mb, mr, sizeBlue, sizeRed;
    float noDmg;
    Specimens *pkmBlue, *pkmRed, *firstPkm, *secPkm;

    FairGame(blue);
    FairGame(red);

    mb = 0;
    mr = 0;
    sizeBlue = GetTeamSize(blue);
    sizeRed = GetTeamSize(red);

    while((mb < sizeBlue) && (mr < sizeRed)){
        pkmBlue = GetTeamMember(blue, mb);
        pkmRed = GetTeamMember(red, mr);
        firstPkm = Fastest(pkmBlue, pkmRed);

        if(firstPkm != NULL){
            if(firstPkm == pkmBlue){
                secPkm = pkmRed;
            }else{
                secPkm = pkmBlue;
            }

            noDmg = 0;

            while((!IsFainted(firstPkm)) && (!IsFainted(secPkm))){
                if(!IsFainted(firstPkm)){
                    TurnAtk(firstPkm, secPkm, &noDmg);
                }
                if(!IsFainted(secPkm)){
                    TurnAtk(secPkm, firstPkm, &noDmg);
                }
            }

            if(IsFainted(pkmBlue)) mb++;
            if(IsFainted(pkmRed)) mr++;
        }
    }
}

void Combat(List *Combats, Trainer *blue, Trainer *red){
    if((Combats != NULL) && (blue != NULL) && (red != NULL) && (blue != red) &&
       (GetTeamSize(blue) > 0) && (GetTeamSize(red) > 0)){
        Simulate(blue, red);
        InnerResult(Combats, blue, red);
    }
}

int LiveCombat(Trainer *blue, Trainer *red, int numbCombat){
    int winId;

    winId = 0;

    if((blue != NULL) && (red != NULL) && (blue != red) && (numbCombat > 0) &&
       (GetTeamSize(blue) > 0) && (GetTeamSize(red) > 0)){
        Simulate(blue, red);
        winId = ApplyResult(blue, red, numbCombat);
    }

    return winId;
}

int GetNumCombat(Result *res){
    return (res != NULL) ? res->numbCombat : 0;
}

int GetBlueId(Result *res){
    return (res != NULL) ? res->blueId : 0;
}

int GetRedId(Result *res){
    return (res != NULL) ? res->redId : 0;
}

int GetWinnerId(Result *res){
    return (res != NULL) ? res->winnerId : 0;
}

int GetLoserId(Result *res){
    if((res == NULL) || (res->winnerId == 0)){
        return 0;
    }

    return (res->winnerId == res->blueId) ? res->redId : res->blueId;
}

Result *GetResultCombat(List *Combats, size_t pos){
    return SearchInPosition(Combats, pos);    
}

void SetResult(List *Combats, Trainer *blue, Trainer *red){
    if((Combats != NULL) && (blue != NULL) && (red != NULL) && (blue != red) &&
       (GetTeamSize(blue) > 0) && (GetTeamSize(red) > 0)){
        InnerResult(Combats, blue, red);
    }
}
