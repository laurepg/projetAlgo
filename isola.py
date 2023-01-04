##
# @file isola.py
# @author Laure Peng
# @brief Ce fichier contient l'ensemble du code et des fonctions du jeu Isola en Python.
# @version 1.0

from random import*

'''
Structure d'une case {
    'positionNL' : (int),   # entre 1 et NL
    'positionNC' : (int),   # entre 1 et NC
    'occupee' : (int),      # 0: non-occupée, 1: Pion1, 2: Pion2
    'detruite' : (chr)     # '0': non-détruite, 'D': détruite
}

'''

##
# @fn choixAdversaire
#
def choixAdversaire():
    """ Le joueur 1 choisit si son adversaire est un humain ou un ordinateur
    @return choix : 1 si l'adversaire est humain, 2 si c'est un ordinateur
    """
    choix = 0

    while (choix != 1 and choix != 2):
        print("----- Joueur adverse -----")
        print("1 - Humain")
        print("2 - Ordinateur")
        choix = int(input("Entrer votre choix : "))
    
    print()
    return choix

##
# @fn initPlateau
#
def initPlateau():
    """ Initialisation du plateau en fonction du nombre de lignes et de colonnes
    @param NL   Nombre de lignes du plateau
    @param NC   Nombre de colonnes du plateau
    @return plateau : un tableau à 2 dimensions contenant un dictionnaire
    @return NL : nombre de lignes du plateau
    @return NC : nombre de colonnes du plateau
    """
    # Saisie de la taille du plateau
    print("----- Taille du plateau -----")
    NL = int(input("Entrer le nombre de lignes : "))
    NC = int(input("Entrer le nombre de colonnes : "))

    # Vérifications
    while (NL >= NC):
        print("NL doit être inférieur à NC.")
        NC = int(input(f"Entrer le nombre de colonnes (>{NL}) : "))

    plateau = list()
    for i in range(NL):
        lignePlateau = []
        for j in range(NC):
            caseP = {
                     'positionNL' : i+1,
                     'positionNC' : j+1,
                     'occupee' : 0,
                     'detruite' : '0'
                    }
            lignePlateau.append(caseP)
        plateau.append(lignePlateau)

    print()
    return plateau, NL, NC

##
# @fn afficherPlateau
#
def afficherPlateau(plateau, NL, NC):
    """ Affichage du plateau
    @param plateau
    @param NL   Nombre de lignes du plateau
    @param NC   Nombre de colonnes du plateau
    """
    # Affichage de "-------- Plateau --------"
    print("\n  ", end="")
    for i in range(5):
        for j in range(0, NC, 2):
            print("-", end="")

    print(" Plateau ", end="")

    for i in range(5):
        for j in range(0, NC, 2):
            print("-", end="")  

    print("\n ", end="")

    # Afficher les numéros de colonnes
    for j in range(NC):
        print(f'     {j+1}', end="")
    print("")
    print("    ", end="")

    for j in range(NC):
        print("------", end="")
    print()

    # Afficher les numéros de lignes et l'état des cases (occupation et destruction)
    for i in range(NL):
        print(f'{i+1} | ', end="")  
        for j in range(NC):
            print(f"[{plateau[i][j]['occupee']},{plateau[i][j]['detruite']}] ", end="")
        print()
    print()

##
# @fn initPions
#
def initPions(NL, NC, plateau):
    """ Initialisation des pions à la position de début du jeu, en fonction du nombre de lignes du plateau.
    @param NL   Nombre de lignes du plateau
    @param NC   Nombre de colonnes du plateau
    @param plateau
    @return tabPions : Tableau de pions 
    """
    tabPions = list()
    NLsur2 = int(NL/2)

    if (NL%2 == 0):
        # Pion du joueur 1
        plateau[NLsur2][0]['occupee'] = 1
        tabPions.append(plateau[NLsur2][0])

        # Pion du joueur 2
        plateau[NLsur2-1][NC-1]['occupee'] = 2
        tabPions.append(plateau[NLsur2-1][NC-1])
    else:
        # Pion du joueur 1
        plateau[NL-1][0]['occupee'] = 1
        tabPions.append(plateau[NL-1][0])

        # Pion du joueur 2
        plateau[0][NC-1]['occupee'] = 2
        tabPions.append(plateau[0][NC-1])
    
    return tabPions

##
# @fn saisirCase
#
def saisirCase(NL, NC):
    """ Saisie d'une case
    @param NL   Nombre de lignes du plateau
    @param NC   Nombre de colonnes du plateau
    @return caseJouee : un dictionnaire contenant les numéros de ligne et de colonne de la case saisie
    """
    caseJouee = dict()

    caseJouee['positionNL'] = int(input("Numéro de ligne : "))
    while (caseJouee['positionNL'] < 1 or caseJouee['positionNL'] > NL):
        print(f'Le numéro de ligne doit être entre 1 et {NL}.')
        caseJouee['positionNL'] = int(input("Numéro de ligne : "))

    caseJouee['positionNC'] = int(input("Numéro de colonne : "))
    while (caseJouee['positionNC'] < 1 or caseJouee['positionNC'] > NC):
        print(f'Le numéro de colonne doit être entre 1 et {NC}.')
        caseJouee['positionNC'] = int(input("Numéro de colonne : "))
    
    print()
    return caseJouee

##
# @fn afficherCase
#
def afficherCase(caseA):
    """ Affichage de la position d'une case
    @param caseA    La case à afficher
    """
    print(f"Vous êtes sur la case ({caseA['positionNL']},{caseA['positionNC']}).")

##
# @fn caseDispo
#
def caseDispo(caseDepart, plateau, NL, NC):
    """ Récupère les cases sur lesquelles le pion peut se déplacer.
    @param caseDepart   Case de départ du pion (dictionnaire contenant les numéros de lignes et de colonnes de la case concernée)
    @param plateau  
    @param NL           Nombre de lignes du plateau
    @param NC           Nombre de colonnes du plateau
    @return caseDispon : tableau contenant les cases disponibles
    @return nbCase : nombre de cases disponibles
    """
    caseEtudiee = dict()
    caseDispon = list()
    nbCase = 0

    tabNL = [caseDepart['positionNL']-1, caseDepart['positionNL'], caseDepart['positionNL']+1]
    tabNC = [caseDepart['positionNC']-1, caseDepart['positionNC'], caseDepart['positionNC']+1]

    for i in range(3):
        if (tabNL[i] > 0 and tabNL[i] <= NL):
            for j in range(3):
                if (tabNC[j] > 0 and tabNC[j] <= NC):
                    caseEtudiee = plateau[tabNL[i]-1][tabNC[j]-1]

                    if (caseEtudiee['occupee'] == 0 and caseEtudiee['detruite'] == '0' and (caseEtudiee['positionNL'] != caseDepart['positionNL'] or caseEtudiee['positionNC'] != caseDepart['positionNC'])):
                        caseDispon.append(caseEtudiee)
                        nbCase += 1
    
    return caseDispon, nbCase

##
# @fn afficherCaseDispo
#
def afficherCaseDispo(caseDispon, nbCaseDispo):
    """ Affichage des cases disponibles
    @param caseDispon   Tableau des cases disponibles
    @param nbCaseDispo  Nombre de cases disponibles
    """
    print("Cases disponibles : ")
    if (nbCaseDispo > 0):
        for i in range(nbCaseDispo):
            print(f"\tCase {i+1} : ({caseDispon[i]['positionNL']},{caseDispon[i]['positionNC']})")
        print()
    else:
        print("Il n'y a pas de cases disponibles.")

##
# @fn detruireCase
#
def detruireCase(plateau, NL, NC):
    """ Le joueur choisit une case qui sera détruite
    @param plateau
    @param NL   Nombre de lignes du plateau
    @param NC   Nombre de colonnes du plateau
    @return plateau
    """
    caseD = saisirCase(NL, NC)
    caseADetruire = plateau[caseD['positionNL']-1][caseD['positionNC']-1]

    while (caseADetruire['occupee'] != 0 or caseADetruire['detruite'] == 'D'):
        if (caseADetruire['occupee'] != 0):
            print("La case est occupée.")
        
        if (caseADetruire['detruite'] == 'D'):
            print("La case est déjà détruite.")
        
        caseD = saisirCase(NL, NC)
        caseADetruire = plateau[caseD['positionNL']-1][caseD['positionNC']-1]
    
    plateau[caseD['positionNL']-1][caseD['positionNC']-1]['detruite'] = 'D'

    return plateau

##
# @fn generAlea
#
def generAlea(maxValue):
    """ Un entier, entre 0 et maxValue-1 inclus, est généré aléatoirement. La fonction est utilisée lorsque le joueur 2 est un ordinateur.
    @param maxValue La valeur maximale que la fonction puisse générer
    @return un entier
    """
    return int(random()*maxValue)

##
# @fn detruireCaseOrdi
#
def detruireCaseOrdi(plateau, NL, NC):
    """ Lorsque l'adversaire est un ordinateur, la case à détruire est choisie aléatoirement. Avant la destruction de cette case, on vérifie que cette case nest ni occupée, ni détruite. Si c'est le cas, une autre case est choisie aléatoirement est vérifiée de nouveau.
    @param plateau
    @param NL       Nombre de lignes du plateau
    @param NC       Nombre de colonnes du plateau
    @return plateau
    """
    caseD = dict()

    caseD['positionNL'] = generAlea(NL)
    caseD['positionNC'] = generAlea(NC)

    caseADetruire = plateau[caseD['positionNL']][caseD['positionNC']]

    while (caseADetruire['occupee'] != 0 or caseADetruire['detruite'] == 'D'):
        caseD['positionNL'] = generAlea(NL)
        caseD['positionNC'] = generAlea(NC)

        caseADetruire = plateau[caseD['positionNL']][caseD['positionNC']]

    plateau[caseD['positionNL']][caseD['positionNC']]['detruite'] = 'D'
    print(f"Le joueur 2 détruit la case ({caseD['positionNL']+1},{caseD['positionNC']+1}).")

    return plateau

##
# @fn verifCaseDispo
#
def verifCaseDispo(choixCase, caseDispon, nbCaseDispo, NL, NC):
    """ Vérifie que la case choisie par le joueur fasse partie des cases disponibles.
    @param choixCase    Case choisie par le joueur (dictionnaire contenant les numéros de ligne et de colonne de la case concernée)
    @param caseDispon   Tableau contenant les cases disponibles
    @param nbCaseDispo  Nombre de cases disponibles
    @param NL           Nombre de lignes du plateau
    @param NC           Nombre de colonnes du plateau
    @return choixCase : la case choisie
    """
    trouveCase = 0

    while (trouveCase == 0):
        j = 0
        while (j < nbCaseDispo):
            if (choixCase['positionNL'] == caseDispon[j]['positionNL'] and choixCase['positionNC'] == caseDispon[j]['positionNC']):
                trouveCase = 1
            j+=1
        
        if (trouveCase != 1):
            print("Le pion ne peut pas se déplacer sur cette case. Déplacez votre pion vers une case libre adjacente ou touchant la case de départ par un coin.")
            afficherCaseDispo(caseDispon, nbCaseDispo)
            choixCase = saisirCase(NL, NC)
    
    return choixCase

##
# @fn faireTour
#
def faireTour(plateau, tabPions, NL, NC, choixAdvers):
    """ On effectue un tour de jeu dans laquelle les 2 joueurs déplacent leur pion et choisissent une case à détruire.
    @param plateau
    @param tabPions     Tableau de 2 pions
    @param NL           Nombre de lignes du plateau
    @param NC           Nombre de colonnes du plateau
    @param choixAdvers  1 si l'adversaire est un humain, 2 si c'est un ordinateur
    @return 1 s'il y a un gagnant, 0 sinon
    @return plateau
    @return tabPions
    """
    i = 0
    numGagnant = 0
    numPerdant = 0

    caseDispon, nbCaseDispo = caseDispo(tabPions[0], plateau, NL, NC)

    if (nbCaseDispo == 0):
        return 1, plateau, tabPions

    # un tour pour chaque joueur
    while ((i<2 and choixAdvers == 1) or (i<1 and choixAdvers == 2)):
        print(f'----- JOUEUR {i+1} -----')
        print()

        afficherCase(tabPions[i])
        print("Déplacez votre pion.")
        afficherCaseDispo(caseDispon, nbCaseDispo)
        pion = saisirCase(NL, NC)

        # Vérifier que la case choisie par le joueur fasse partie des cases disponibles.
        pion = verifCaseDispo(pion, caseDispon, nbCaseDispo, NL, NC)

        # Rendre l'ancienne cases non-occupée
        plateau[tabPions[i]['positionNL']-1][tabPions[i]['positionNC']-1]['occupee'] = 0
        tabPions[i] = pion
        # Rendre la case actuelle ocucupée
        plateau[pion['positionNL']-1][pion['positionNC']-1]['occupee'] = i+1

        afficherPlateau(plateau, NL, NC)

        print()
        print("Choisissez une case à détruire.")

        plateau = detruireCase(plateau, NL, NC)
        afficherPlateau(plateau, NL, NC)

        print()
        i+=1

        for j in range(2):
            caseDispon, nbCaseDispo = caseDispo(tabPions[j], plateau, NL, NC)

            if (nbCaseDispo == 0):
                if (j==0):
                    numGagnant = 2
                    numPerdant = 1
                if (j==1):
                    numGagnant = 1
                    numPerdant = 2
                print(f'Le joueur {numPerdant} ne peut plus se déplacer.')
                print(f'Le joueur {numGagnant} a donc gagné !')
                return 1, plateau, tabPions

    if (choixAdvers == 2):
        print(f'----- JOUEUR {i+1} -----')
        print()

        print(f"Le joueur 2 est sur la case ({tabPions[i]['positionNL']},{tabPions[i]['positionNC']}).")

        # Vérifier les choix possbles de l'ordinateur
        #afficherCaseDispo(caseDispon, nbCaseDispo)
        alea = generAlea(nbCaseDispo-1)
        pion = caseDispon[alea]
        print(f"Le joueur 2 déplace son pion sur la case ({pion['positionNL']},{pion['positionNC']}).")

        # Rendre l'ancienne case non-occupée
        plateau[tabPions[i]['positionNL']-1][tabPions[i]['positionNC']-1]['occupee'] = 0
        tabPions[i] = pion
        # Rendre la case actuelle occupée
        plateau[pion['positionNL']-1][pion['positionNC']-1]['occupee'] = i+1

        print()
        afficherPlateau(plateau, NL, NC)
        plateau = detruireCaseOrdi(plateau, NL, NC)

        afficherPlateau(plateau, NL, NC)
        print()

        for j in range(2):
            caseDispon, nbCaseDispo = caseDispo(tabPions[j], plateau, NL, NC)

            if (nbCaseDispo == 0):
                if (j==0):
                    numGagnant = 2
                    numPerdant = 1
                if (j==1):
                    numGagnant = 1
                    numPerdant = 2
                
                print(f"Le joueur {numPerdant} ne peut plus se déplacer.")
                print(f"Le joueur {numGagnant} a donc gagné ! ")
                return 1, plateau, tabPions

    return 0, plateau, tabPions


################
##### Main
NL = int()
NC = int()
gagnant = 0

plateau, NL, NC = initPlateau()

choixAdvers = choixAdversaire()

afficherPlateau(plateau, NL, NC)

tabPions = initPions(NL, NC, plateau)
afficherPlateau(plateau, NL, NC)

while (gagnant != 1):
    gagnant, plateau, tabPions = faireTour(plateau, tabPions, NL, NC, choixAdvers)

# Fin du code