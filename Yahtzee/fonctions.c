#include "fonctions.h"
#include <time.h>

// Le fameux srand
void init(void) {
	srand((unsigned int)time(NULL));
}

//-----------------------------------------------------------------------------
// La partie
void partie(int nbrJoueurs) {

	// D�claration des variables
	int nbrCoups = 0; // Compteur de coups
	int i = 0; // Indice du joueur � qui c'est le tour
	int des[5] = { 1 , 2 , 3 , 4 , 5 }; // Stocke la valeur des D�s
	
	// Les variables pour les scanf_s
	int choixRelance; // Relancer?
	int nbDes; // Combien de d�s?
	int desRelance; // Le quel?
	int choixComb; // Quelle combinaison?
	
	// Le tableau de pointeurs sur des structures (initialis� a 0 gr�ce au calloc)
	struct joueur* listeJoueur = (joueur*)calloc(nbrJoueurs, sizeof(*listeJoueur)); 

	while (nbrCoups < nbrJoueurs * 12) { // Tant qu'il reste des coups � jouer
										 // nombre de joueurs * nombre de combinaisons = nomre de coups total

		//On initialise ces variables a chaque tour		
		int listeOccurences[6] = { 0 , 0 , 0 , 0 , 0 , 0 }; // La liste des occurences de chaque valeur des d�s
		listeJoueur[i].cptRelance = 2; // On reset compteur de relance
		choixRelance = 1; // On met 1 pour poser une premiere fois la question
		choixComb = 0; // On reset son choix de combinaison

		// Et on lance les D�s
		lancerDes(des);	

		// Si il le peut, on propose � l'utilisateur de relancer un ou plusieurs D�s
		while ((choixRelance != 2) && (listeJoueur[i].cptRelance != 0)) {
				afficher(listeJoueur[i], i, des); // On affiche l'interface, on l'affichera r�guli�rement pour �viter d'avoir � scroll
				printf(" Voulez vous relancer des Des? 1-Oui, 2-Non\n");
				scanf_s(" %d", &choixRelance);

				// Si il veut relancer, on lui demande combien de D�s
				if (choixRelance == 1) {
					listeJoueur[i].cptRelance -= 1; // On enleve une relance

					afficher(listeJoueur[i], i, des);
					
					do {
						printf(" Combien de Des voulez-vous relancer?\n");
						scanf_s(" %d", &nbDes);
					} while ((nbDes > 5) || (nbDes < 0));

					afficher(listeJoueur[i], i, des);

					// On lui demande quel D� relancer
					for (int d = 1; d < nbDes + 1; d++) { 
						do {
							printf(" Entrez le numero du De que vous voulez relancer (%d/%d)\n", d, nbDes);
							scanf_s(" %d", &desRelance);
						} while ((desRelance < 1) || (desRelance > 5));
						relancerDe(desRelance, des); // On relance le D� que l'utilisateur a choisi
					}

					afficher(listeJoueur[i], i, des);
				}
		}

		// On demande au joueur quelle combinaison il souhaite valider
		do{
			afficher(listeJoueur[i], i, des);
			printf(" Entrez le numero de la combinaison que vous souhaitez valider :\n");
			scanf_s(" %d", &choixComb);
		} while ((listeJoueur[i].combValide[choixComb-1] == 1)||(choixComb>12)||(choixComb<1)); // V�rifie si la combinaison n'a pas d�j� �t� valid�e et qu'elle existe

		int indiceListe2 = choixComb - 7; // L'indice d'une combinaison de la liste 2
		listerDes(listeOccurences, des); // On liste les occurences de D�s
		listeJoueur[i].combValide[choixComb-1] = 1; // On met 1 dans le tableau qui stocke quelles combinaisons ont �t� valid�es

		// Les cases pour chaque choix de combinaison
		switch (choixComb) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		{
			listeJoueur[i].fiche1Joueur[choixComb - 1] = multiple(choixComb, listeOccurences);
		}
		break;

		case 7:
			listeJoueur[i].fiche2Joueur[indiceListe2] = brelan(listeOccurences);
			break;

		case 8:
			listeJoueur[i].fiche2Joueur[indiceListe2] = petiteSuite(listeOccurences);
			break;

		case 9:
			listeJoueur[i].fiche2Joueur[indiceListe2] = grandeSuite(listeOccurences);
			break;

		case 10:
			listeJoueur[i].fiche2Joueur[indiceListe2] = full(listeOccurences);
			break;
		case 11:
			listeJoueur[i].fiche2Joueur[indiceListe2] = carre(listeOccurences);
			break;
		case 12:
			listeJoueur[i].fiche2Joueur[indiceListe2] = yams(listeOccurences);
			break;
		}

		listeJoueur[i].fiche1Joueur[7] = total(listeJoueur[i], 1);// Total Fiche 1

		// La prime	
		if (listeJoueur[i].fiche1Joueur[7] > 62) {
			listeJoueur[i].fiche1Joueur[6] = 35; 
			listeJoueur[i].fiche1Joueur[7] = total(listeJoueur[i], 1); // On recalcule le Total 1
		}
		listeJoueur[i].fiche2Joueur[6] = total(listeJoueur[i], 2);// Total Fiche 2

		afficher(listeJoueur[i], i, des);

		system("pause"); // On tape sur une touche pour passer au joueur suivant
		i = (i + 1) % nbrJoueurs; // Joueur suivant
		nbrCoups++; // Le compteur de coups jou�s
	}
	free(listeJoueur);
	system("pause");
}
//-----------------------------------------------------------------------------
// On lance les d�s
void lancerDes(int *L) {
	for (int i = 0; i < 5; i++) {
		L[i] = (rand()%6) + 1;
	}
}

// On relance un d�
void relancerDe(int de, int *L) {
	L[de-1] = (rand() % 6) + 1;
}
//-----------------------------------------------------------------------------
// Le calcul des Scores :

// Retourne la somme des n premiers �lement d'un tableau:
int total(struct joueur j, int num) {
	int res = 0;
	if (num == 1) {
		for (int i = 0; i <= 6; i++) {
			res = res + j.fiche1Joueur[i];
		}
	}
	else if (num == 2){
		for (int i = 0; i <= 5; i++) {
			res = res + j.fiche2Joueur[i];
		}
	}
	return res;
}

// On liste les occurences des d�s, je boucle sur cette liste pour chaque calcul de score de la fiche 2
void listerDes(int *L, int *LDes) {
	for (int i = 0; i < 5; i++) {
		L[LDes[i]-1] += 1;
	}
}

// Fiche 1:
// Le score des 6 premi�res combinaisons
int multiple(int valeur, int *L) {
	return L[valeur - 1] * valeur; // est donn� part: le nombre de d�s aillant la valeur choisie * la valeur choisie
}

// Fiche 2:
// Retourne la somme des valeur des d�s si il y a un brelan
int brelan(int *L) { 
	for (int i = 0; i < 6; i++) {
		if (L[i] >= 3) {
			return sommeDes(L);
		}	 
	}
	return 0;
}

// Retourne 15 si il y a une Petite suite
int petiteSuite(int *L) {
	for (int i = 0; i < 3; i++) {
		if (L[i] && L[i + 1] && L[i + 2] && L[i + 3] >= 1) {
			return 15;
		}
	}
	return 0;
}

// Retourne 20 si il y a une Grande suite
int grandeSuite(int *L) {
	for (int i = 0; i < 6; i++) {
		if(L[i] > 1){
			return 0;
		}
	}
	if ((L[0] == 0)||(L[5] == 0)){
		return 20;
	}
	return 0;
}

// Retourne 30 si il y a un Full
int full(int *L) {
	if (brelan(L) != 0) {
		for (int i = 0; i < 6; i++) {
			if(L[i] == 2){
				return 30;
			}
		}
	}
	return 0;
}

// Retourne la somme des valeurs des D�s + 20 si il y a un Carr�
int carre(int *L) {
	for (int i = 0; i < 6; i++) {
		if (L[i] == 4) {
			return sommeDes(L) + 20;
		}
	}
	return 0;
}

// Retourne 50 si il y a un Yams
int yams(int *L) {
	for (int i = 0; i < 6; i++) {
		if (L[i] == 5) {
			return 50;
		}
	}
	return 0;
}

// Retourne la somme des valeurs des D�s
int sommeDes(int *L) {
	int res = 0;
	for (int i = 0; i < 6; i++) {
		res = res + L[i] * (i+1);
	}
	return res;
}


void afficher(struct joueur j, int num, int *des) {
	system("cls");
	// L'affichage des fiches
	printf(" Joueur %d\n", num + 1);
	printf("-------------------------------------------\n");
	printf("%d  1 - As: %d\n", j.combValide[0], j.fiche1Joueur[0]); // On affiche 0 � gauche si la combinaison n'a pas encore �t� faite, 1 sinon
	printf("%d  2 - Deux: %d\n", j.combValide[1], j.fiche1Joueur[1]);
	printf("%d  3 - Trois: %d\n", j.combValide[2], j.fiche1Joueur[2]);
	printf("%d  4 - Quatre: %d\n", j.combValide[3], j.fiche1Joueur[3]);
	printf("%d  5 - Cinq: %d\n", j.combValide[4], j.fiche1Joueur[4]);
	printf("%d  6 - Six: %d\n", j.combValide[5], j.fiche1Joueur[5]);
	printf("    Prime: %d\n", j.fiche1Joueur[6]);
	printf("    Total 1: %d\n", j.fiche1Joueur[7]);
	printf("-------------------------------------------\n");
	printf("%d  7 - Brelan: %d\n", j.combValide[6], j.fiche2Joueur[0]);
	printf("%d  8 - Petite suite: %d\n", j.combValide[7], j.fiche2Joueur[1]);
	printf("%d  9 - Grande suite: %d\n", j.combValide[8], j.fiche2Joueur[2]);
	printf("%d 10 - Full: %d\n", j.combValide[9], j.fiche2Joueur[3]);
	printf("%d 11 - Carre: %d\n", j.combValide[10], j.fiche2Joueur[4]);
	printf("%d 12 - Yams: %d\n", j.combValide[11], j.fiche2Joueur[5]);
	printf("    Total 2: %d\n", j.fiche2Joueur[6]);
	printf("-------------------------------------------\n");
	printf(" TOTAL : %d\n", j.fiche2Joueur[6] + j.fiche1Joueur[7]);
	// Les D�s
	printf("-------------------------------------------\n");
	printf(" D1: %d, D2: %d, D3: %d, D4: %d, D5: %d\n", des[0], des[1], des[2], des[3], des[4]);
	printf("-------------------------------------------\n");
}