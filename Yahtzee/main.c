#include "fonctions.h"

int main(void) {
	
	int nbrJoueurs = 2;
	init();

	system("cls");
	printf("Nombre de joueurs? > 2\n");
	scanf_s("%d", &nbrJoueurs);

	partie(nbrJoueurs);

	system("pause");
}