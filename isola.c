/**
 * @file isola.c
 * @author Laure Peng
 * @brief Ce fichier contient l'ensemble du code et des fonctions du jeu Isola en C.
 * @version 1.0 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @struct case_t
 * @brief Structure d'une case
 */
typedef struct casePlateau {
    int positionNL;
    int positionNC;
    int occupee; // 0: non-occupee; 1: Pion1; 2: Pion2
    char detruite; // '0': non-detruite; 'D': detruite
} case_t; 

/**
 * @fn void freePlateau (case_t **plateau, int NL)
 * @brief Libere la memoire du plateau
 * 
 * @param plateau Tableau a 2 dimensions de case_t
 * @param NL Nombre de lignes du plateau
 */
void freePlateau(case_t **plateau, int NL) {
    int i;

    for (i=0; i<NL; i++) {  
        free(plateau[i]); // libere la memoire de la ligne i
    }
    free(plateau); // libere la memoire pour les pointeurs des lignes
} 

/**
 * @fn case_t ** allocPlateau (int NL, int NC)
 * @brief Allocation dynamique de la memoire du plateau
 * 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @return plateau
 */
case_t ** allocPlateau(int NL, int NC) {
    case_t **plateau = NULL;
    int i;

    plateau = (case_t **) malloc(NL * sizeof(case_t *));  // memoire pour les pointeurs sur les lignes

    if (plateau) {
        for (i = 0; i<NL; i++) {
            plateau[i] = (case_t *) malloc(NC * sizeof(case_t)); // memoire pour les cases de la ligne i

            if (plateau[i] == NULL) { // liberer les memoires allouees precedemment si l'allocation courante a echoue
                freePlateau(plateau, i);
                plateau = NULL;
            }
        }
    }

    return plateau;
}

/**
 * @fn int choixAdversaire (void)
 * @brief Le joueur 1 choisit si son adversaire est un humain ou un ordinateur.
 * 
 * @return 1 si l'adversaire est un humain, 2 si c'est un ordinateur
 */
int choixAdversaire(void) {
    int choix = 0;

    while (choix != 1 && choix != 2) {
        printf("----- Joueur adverse -----\n");
        printf("1 - Humain\n");
        printf("2 - Ordinateur\n");
        printf("Entrer votre choix : ");
        scanf("%d", &choix);
    }
    printf("\n");

    return choix;
}

/**
 * @fn case_t ** initPlateau(int *ptNL, int *ptNC)
 * @brief On demande la taille du plateau au joueur 1. A partir du nombre de lignes et de colonnes, le plateau est initalise.
 * 
 * @param ptNL Pointeur sur le nombre de lignes du plateau
 * @param ptNC Pointeur sur le nombre de colonnes du plateau
 * @return plateau 
 */
case_t ** initPlateau(int *ptNL, int *ptNC) {
    int i, j;
    case_t **plateau = NULL;

    // Saisie de la taille du plateau
    printf("----- Taille du plateau -----\n");
    printf("Entrer le nombre de lignes : ");
    scanf("%d", ptNL);
    printf("Entrer le nombre de colonnes : ");
    scanf("%d", ptNC);

    // Verifications
    while (*ptNL >= *ptNC) {
        printf("NL doit etre inferieur a NC.\n");
        printf("Entrer le nombre de colonnes (>%d) : ", *ptNL);
        scanf("%d", ptNC);
    }

    plateau = allocPlateau(*ptNL, *ptNC);

    // Initialisation du plateau
    for (i=0; i<*ptNL; i++) {
        for (j=0; j<*ptNC; j++) {
            plateau[i][j].positionNL = i+1;
            plateau[i][j].positionNC = j+1;
            plateau[i][j].occupee = 0;
            plateau[i][j].detruite = '0';
        }
    }

    printf("\n");
    return plateau;
}


/**
 * @fn void afficherPlateau (case_t **plateau, int NL, int NC)
 * @brief Affichage du plateau
 * 
 * @param plateau 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 */
void afficherPlateau(case_t **plateau, int NL, int NC) {
    int i, j;

    // Affichage de "---------- Plateau ----------"
    printf("   ");
    for (i=0; i<5; i++) {
        for (j=0; j<NC; j+=2) {
            printf("-");
        }
    }
    printf(" Plateau ");
    for (i=0; i<5; i++) {
        for (j=0; j<NC; j+=2) {
            printf("-");
        }
    }
    printf("\n  ");

    // afficher les numeros de colonnes
    for (j=0; j<NC; j++) {
        printf("%6d", j+1);
    }
    printf("\n    ");

    for (j=0; j<NC; j++) {
        printf("------");
    }
    printf("\n");

    // afficher les numero de ligne et l'etat des cases
    for (i=0; i<NL; i++) {
        printf("%2d | ", i+1);
        for (j=0; j<NC; j++) {
            printf("[%d,%c] ", plateau[i][j].occupee, plateau[i][j].detruite);
        }
        printf("\n");
    }
    printf("\n");

}

/**
 * @fn void initPions (int *ptNL, int *ptNC, case_t **plateau, case_t *tabPions)
 * @brief Initialisation des pions a la position de debut du jeu, en fonction du nombre de lignes du plateau.
 * 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @param plateau 
 * @param tabPions Tableau de pions
 */
void initPions(int NL, int NC, case_t **plateau, case_t *tabPions) {
    int NLsur2 = NL/2;

    if (NL%2 == 0) {
        // Pion du joueur 1
        plateau[NLsur2][0].occupee = 1;
        tabPions[0] = plateau[NLsur2][0];

        // Pion du joueur 2
        plateau[NLsur2-1][NC-1].occupee = 2;
        tabPions[1] = plateau[NLsur2-1][NC-1];
    }
    else {
        // Pion du joueur 1
        plateau[NL-1][0].occupee = 1;
        tabPions[0] = plateau[NL-1][0];

        //Pion du joueur 2
        plateau[0][NC-1].occupee = 2;
        tabPions[1] = plateau[0][NC-1];
    }
}

/**
 * @fn case_t saisirCase (int NL, int NC)
 * @brief Saisie d'une case 
 * 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @return caseJouee La case saisie par le joueur
 */
case_t saisirCase(int NL, int NC) {
    case_t caseJouee;

    printf("Numero de ligne : ");
    scanf("%d", &caseJouee.positionNL);
    while (caseJouee.positionNL < 1 || caseJouee.positionNL > NL) {
        printf("Le numero de ligne doit etre entre 1 et %d.\n", NL);
        printf("Numero de ligne : ");
        scanf("%d", &caseJouee.positionNL);
    }
    printf("Numero de colonne : ");
    scanf("%d", &caseJouee.positionNC);
    while (caseJouee.positionNC < 1 || caseJouee.positionNC > NC) {
        printf("Le numero de colonne doit etre entre 1 et %d.\n", NC);
        printf("Numero de colonne : ");
        scanf("%d", &caseJouee.positionNC);
    }
    printf("\n");

    return caseJouee;
}

/**
 * @fn void afficherCase (case_t caseA)
 * @brief Affichage de la position d'une case.
 * 
 * @param caseA La case a afficher
 */
void afficherCase(case_t caseA) {
    printf("Vous etes sur la case (%d,%d).\n", caseA.positionNL, caseA.positionNC);
}

/**
 * @fn int caseDispo (case_t *caseDepart, case_t **plateau, case_t caseDispon[], int NL, int NC)
 * @brief Recupere les cases sur lesquelles le pion peut se deplacer.
 * 
 * @param caseDepart Case de depart du pion
 * @param plateau 
 * @param caseDispon Tableau des cases disponibles
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @return nbCase Nombre de cases disponibles
 */
int caseDispo(case_t *caseDepart, case_t **plateau, case_t caseDispon[], int NL, int NC) {
    int nbCase, i, j;
    case_t caseEtudiee;
  
    nbCase = 0;

    // Initialisation du tableau caseDispon
    for (i=0; i<8; i++) {
        caseDispon[i].positionNL = -1;
        caseDispon[i].positionNC = -1;
    }

    int tabNL[3] = {caseDepart->positionNL-1, caseDepart->positionNL, caseDepart->positionNL+1};
    int tabNC[3] = {caseDepart->positionNC-1, caseDepart->positionNC, caseDepart->positionNC+1};

    for (i=0; i<3; i++) {
        if ((tabNL[i] > 0) && (tabNL[i] <= NL)) {
            for (j=0; j<3; j++) {
                if ((tabNC[j] > 0) && (tabNC[j] <= NC)) {
                    caseEtudiee = plateau[tabNL[i]-1][tabNC[j]-1];
                    
                    if ((caseEtudiee.occupee == 0) && (caseEtudiee.detruite == '0') && ((caseEtudiee.positionNL != caseDepart->positionNL) || (caseEtudiee.positionNC != caseDepart->positionNC))) {
                        caseDispon[nbCase] = caseEtudiee;
                        nbCase++;
                    }
                }
            }
        }
    }

    return nbCase;
}

/**
 * void afficherCaseDispo (case_t *caseDispon, int nbCaseDispo)
 * @brief Affichage des cases disponibles
 * 
 * @param caseDispon Tableau des cases disponibles
 * @param nbCaseDispo Nombre de cases disponibles
 */
void afficherCaseDispo(case_t *caseDispon, int nbCaseDispo) {
    printf("Case disponibles : \n");
    if (nbCaseDispo > 0) { 
        for (int i=0; i<nbCaseDispo; i++) {
            printf("\tCase %d : (%d,%d)\n", i+1, caseDispon[i].positionNL, caseDispon[i].positionNC);
        }
        printf("\n");
    }
    else {
        printf("Il n'y a pas de cases disponibles.\n");
    }
}

/**
 * @fn void detruireCase (case_t **plateau, int NL, int NC)
 * @brief Le joueur choisit une case qui sera detruite.
 * 
 * @param plateau 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 */
void detruireCase(case_t **plateau, int NL, int NC) {
    case_t caseD, caseADetruire;

    caseD = saisirCase(NL, NC);
    caseADetruire = plateau[caseD.positionNL-1][caseD.positionNC-1];

    while (caseADetruire.occupee != 0 || caseADetruire.detruite == 'D') {
        if (caseADetruire.occupee != 0) {
            printf("La case est occupee.\n");
        }

        if (caseADetruire.detruite == 'D') {
            printf("La case est deja detruite.\n");
        }

        caseD = saisirCase(NL, NC);
        caseADetruire = plateau[caseD.positionNL-1][caseD.positionNC-1];
    }

    plateau[caseD.positionNL-1][caseD.positionNC-1].detruite = 'D';
}

/**
 * @fn int generAlea (int maxValue)
 * @brief Un entier, entre 0 et maxValue-1 inclus, est genere aleatoirement. La fonction est utilisee lorsque le joueur 2 est un ordinateur.
 * 
 * @param maxValue La valeur maximale que la fonction puisse generer.
 * @return un entier
 */
int generAlea(int maxValue) {
    return (int)(((float) rand()/RAND_MAX)*(maxValue));
}

/**
 * @fn void detruireCaseOrdi (case_t **plateay, int NL, int NC)
 * @brief Lorsque l'adversaire est un ordinateur, la case a detruire est choisie aleatoirement. Avant la destruction de cette case, on verifie que cette case n'est ni occupee, ni detruite. Si c'est le cas, une autre case est choisie aleatoirement et verifiee de nouveau.
 * 
 * @param plateau 
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 */
void detruireCaseOrdi(case_t **plateau, int NL, int NC) {
    case_t caseD, caseADetruire;

    caseD.positionNL = generAlea(NL);
    caseD.positionNC = generAlea(NC);

    caseADetruire = plateau[caseD.positionNL][caseD.positionNC];

    while (caseADetruire.occupee != 0 || caseADetruire.detruite == 'D') {
        caseD.positionNL = generAlea(NL);
        caseD.positionNC = generAlea(NC);

        caseADetruire = plateau[caseD.positionNL][caseD.positionNC];
    }

    plateau[caseD.positionNL][caseD.positionNC].detruite = 'D';
    printf("Le joueur 2 detruit la case (%d,%d).\n", caseD.positionNL+1, caseD.positionNC+1);
}

/**
 * @fn case_t verifCaseDispo (case_t pion, case_t *caseDispon, int nbCaseDispo, int NL, int NC)
 * @brief Verifie que la case choisie par le joueur fasse partie des cases disponibles.
 * 
 * @param choixCase Case choisie par le joueur
 * @param caseDispon Tableau contenant les cases disponibles
 * @param nbCaseDispo Nombre de cases dipsonibles
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @return choixCase Case choisie par le joueur
 */
case_t verifCaseDispo(case_t choixCase, case_t *caseDispon, int nbCaseDispo, int NL, int NC) {
    int trouveCase = 0;

    while (trouveCase == 0) {
        int j = 0;
        while (j < nbCaseDispo) {
            if ((choixCase.positionNL == caseDispon[j].positionNL) && (choixCase.positionNC == caseDispon[j].positionNC)) {
                trouveCase = 1;
            }
            j++;
        }

        if (trouveCase != 1) {
            printf("Le pion ne peut pas se deplacer sur cette case. Deplacez votre pion vers une case libre adjacente ou touchant la case de depart par un coin.\n");
            afficherCaseDispo(caseDispon, nbCaseDispo);
            choixCase = saisirCase(NL, NC);
        }
    }

    return choixCase;
}

/**
 * @fn void faireTour (case_t **plateau, case_t *tabPions, int NL, int NC, int choixAdvers)
 * @brief On effectue un tour de jeu dans laquelle les 2 joueurs deplacent leur pion et choisissent une case a detruire.
 * 
 * @param plateau 
 * @param tabPions Tableau du pion de chaque joueur
 * @param NL Nombre de lignes du plateau
 * @param NC Nombre de colonnes du plateau
 * @param choixAdvers 1 pour un humain, 2 pour un ordinateur
 * @return 1 s'il y a un gagnant, 0 sinon
 */
int faireTour(case_t **plateau, case_t *tabPions, int NL, int NC, int choixAdvers) {
    int i=0, j;
    int numGagnant = 0, numPerdant = 0;
    int nbCaseDispo = 0;
    int alea = 0;
    case_t pion, caseDispon[8];
    
    nbCaseDispo = caseDispo(&tabPions[0], plateau, caseDispon, NL, NC);
    
    if (nbCaseDispo == 0) {

        return 1;
    }

    // un tour pour chaque joueur
    while ((i<2 && choixAdvers == 1) || (i<1 && choixAdvers == 2)) { 
        printf("----- JOUEUR %d -----\n", i+1);
        printf("\n");
            
        afficherCase(tabPions[i]);
        printf("Deplacez votre pion.\n");
        afficherCaseDispo(caseDispon, nbCaseDispo);
        pion = saisirCase(NL, NC);

        // Verifier que la case choisie par le joueur fasse partie des cases disponibles
        pion = verifCaseDispo(pion, caseDispon, nbCaseDispo, NL, NC);

        // Rendre l'ancienne case non-occupee 
        plateau[tabPions[i].positionNL-1][tabPions[i].positionNC-1].occupee = 0;
        tabPions[i] = pion;
        // Rendre la case actuelle occupee
        plateau[pion.positionNL-1][pion.positionNC-1].occupee = i+1;

        afficherPlateau(plateau, NL, NC);

        printf("\n");
        printf("Choisissez une case a detruire.\n");

        detruireCase(plateau, NL, NC);
        afficherPlateau(plateau, NL, NC);

        printf("\n");
        i++;
        for (j=0; j<2; j++) {
            nbCaseDispo = caseDispo(&tabPions[j], plateau, caseDispon, NL, NC);

            if (nbCaseDispo == 0) {
                if (j == 0) {
                    numGagnant = 2;
                    numPerdant = 1;
                }
                if (j == 1) {
                    numGagnant = 1;
                    numPerdant = 2;
                }
                printf("Le joueur %d ne peut plus se deplacer.\n", numPerdant);
                printf("Le joueur %d a donc gagne !\n", numGagnant);
                return 1;
            }
        }
        
    }

    if (choixAdvers == 2) {
        printf("----- JOUEUR %d -----\n", i+1);
        printf("\n");

        printf("Le joueur 2 est sur la case (%d,%d).\n", tabPions[i].positionNL, tabPions[i].positionNC);

        // Verifier les choix possibles de l'ordinateur
        // afficherCaseDispo(caseDispon, nbCaseDispo);
        alea = generAlea(nbCaseDispo-1);
        pion = caseDispon[alea];
        printf("Le joueur 2 deplace son pion sur la case (%d,%d).\n", pion.positionNL, pion.positionNC);

        // Rendre l'ancienne case non-occupee 
        plateau[tabPions[i].positionNL-1][tabPions[i].positionNC-1].occupee = 0;
        tabPions[i] = pion;
        // Rendre la case actuelle occupee
        plateau[pion.positionNL-1][pion.positionNC-1].occupee = i+1;

        printf("\n");
        afficherPlateau(plateau, NL, NC);
        detruireCaseOrdi(plateau, NL, NC);

        afficherPlateau(plateau, NL, NC);
        printf("\n");

        for (j=0; j<2; j++) {
            nbCaseDispo = caseDispo(&tabPions[j], plateau, caseDispon, NL, NC);

            if (nbCaseDispo == 0) {
                if (j == 0) {
                    numGagnant = 2;
                    numPerdant = 1;
                }
                if (j == 1) {
                    numGagnant = 1;
                    numPerdant = 2;
                }
                printf("Le joueur %d ne peut plus se deplacer.\n", numPerdant);
                printf("Le joueur %d a donc gagne !\n", numGagnant);
                return 1;
            }
        }
    }
    
    return 0;
}


int main() {
    case_t **plateau = NULL ;
    int NL, NC;
    int choixAdvers = 0;
    case_t tabPions[2];
    int gagnant = 0;

    plateau = initPlateau(&NL, &NC);

    if (plateau != NULL) { // allocation de memoire reussie
        srand(time(NULL));

        choixAdvers = choixAdversaire();

        afficherPlateau(plateau, NL, NC);

        initPions(NL, NC, plateau, tabPions);
        afficherPlateau(plateau, NL, NC);

        while (gagnant != 1) {
            gagnant = faireTour(plateau, tabPions, NL, NC, choixAdvers);
        }

        freePlateau(plateau, NL); // liberation de la memoire
    }
    
    return 0;
}