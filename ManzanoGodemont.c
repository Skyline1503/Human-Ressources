//PROJET EN C 2021-2022
//SUJETS N° 5 -> RESSOURCES HUMAINESS
//REALISE PAR : 
//				GODEMONT FLAVIAN  LA209224
//				MANZANO  MATHIAS  LA208312

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
char *clear = "cls";
#elif __unix__ || __APPLE__ && __MACH__
char *clear = "clear";
#endif

/* strucut Employe
-------------------------------------------------------------------------------------------------------------------------------------------------------------
	char num[9];			-> Sera le numéro d'employe qui sera unique (9caractéres), 2chiffres, 3premieres lettre du nom et 3premieres lettre du prenom
	char nom[28]; 			-> Sera le nom de l'employé (28 caractéres)
	char prenom[28]; 		-> Sera le prenom de l'employé (28 caractéres)
	char statut[16]; 		-> Sera le statut de l'employé (16 caractéres)
	int jourN; 				-> Sera le jour de naissance (2 caractéres)
	int moisN;				-> Sera le mois de naissance (2 caractéres)
	int anneeN;				-> Sera l'année de naissance (4 caractéres)
	int jourE;				-> Sera le jour d'embauche (2 caractéres)
	int moisE;				-> Sera le mois d'embauche (2 caractéres)
	int anneeE;				-> Sera l'année d'embauche (4 caractéres)
	int anciennete;			-> Sera le nombre d'année d'anciennete (2 caracteres)
	char sexe[2]			-> Sera le sexe de l'employe
	char type[11];			-> Sera le type de l'employé (11 caractéres)
	char fonction[28];		-> Sera la fonction de l'employé (11 caractéres)
	char typeContrat[14];	-> Sera le type de contract de l'employé (11 caractéres)
	int heureSem;			-> Sera le nombre d'heure semaine de l'employé (2 caractéres)
	float salaire;			-> Sera Le salaire de l'employé(8 caractéres)
*/

typedef struct Employe
{
	char num[9];
	char nom[28];
	char prenom[28];
	char statut[15];
	int jourN;
	int moisN;
	int anneeN;
	int jourE;
	int moisE;
	int anneeE;
	int anciennete;
	char sexe[2];
	char type[11];
	char fonction[28];
	char typeContrat[14];
	int heureSem;
	float salaire;
	struct Employe *suivant;
}Employe;

/* strucut Service
-------------------------------------------------------------------------------------------------------------------------------------------------------------
	char nom[28]; 			-> Sera le nom du service (28 caractéres)
	float tarifHoraire;	    -> Le tarif horaire du service (6 caractéres)
	float indexation;       -> L'indexation horaire du service (5 caractéres)
*/

typedef struct Service
{
	char nom[28];
	float tarifHoraire;
	float indexation;
	struct Service *suivant;
}Service;

// DECLARATIONS DES FONCTIONS 
int calculerAnciente(int,int,int);
int lire(char *,int);
void afficherStatut(char *);
void convertirMaju(char*);
float calculSalaire(float,float,Employe *);
void typeContract(Employe *);
void afficherListe(Employe *, int);
void afficherListeService(Service *,int);
void enregistrerListe(Employe *cour,int,Service *,int);
void choixType(char *);
void choixService(Service *, int, Employe *);
void ajouterEmploye(Employe **, int *, Service *, int);
void ajouterService(Service **,int *);
void supprimerEmploye(Employe **,int *);
void supprimerService(Service **, int *);
void anniversaireMois(Employe *, int);
void modifierEmploye(Employe *, int, Service *, int);
void modifierService(Service *, int);
void afficherEmpServ(Employe *, int, Service *, int);
void afficherEmpStatut(Employe *, int);
void modifierStatutEmp(Employe **, int *);
void backup(Employe *, int, Service *, int);
void exporterCSV(Employe *, int, Service *, int);
void rechercheEmploye(Employe *, int);
void menuPrincipal(Employe **, int *, Service **, int *);
void menuEmploye(Employe **, int *, Service **, int *);
void menuService(Employe **, int *, Service **, int *);

/*
	Cette fonction sert a calculer le nombre d'année d'anciennete 
	(duree ou l'employe se trouve dans la societe). Ce qui permettra
	aussi de calculer son salaire net.
*/

int calculerAnciente(int jourE, int moisE, int anneeE)
{
	// DECLARATION DES VARIABLES
	int jourJ, moisJ, anneeJ, nbAnnee;
  	time_t now;
    
	// RENVOIE L'HEURE ACTUEL
	time(&now);
	// L'HEURE EST CONVERTI A L'HEURE LOCAL
	struct tm *local = localtime(&now);      
	jourJ = local->tm_mday;          
	moisJ = local->tm_mon + 1;     
	anneeJ = local->tm_year + 1900; 
	
	// DEFINIE LE NOMBRE D'ANNEE D'ANCIENNETE DE BASE
	nbAnnee=anneeJ-anneeE;
	// CONDITION CALCULER LES ANNEES D'ANCIENNETE EN FONCTION DU MOIS ET DU JOUR
	if(moisE>=moisJ)
	{
		if(jourE>jourJ)
		{
			nbAnnee--;
		}
	}
	// SI L'ANNEE = 0 ALORS IL SERA RENVOYE -1 POUR DIRE QUE CELA FAIS - D'UN AN QU'IL TRAVAIL DANS LA SOVIETE
	if(nbAnnee==0)
	{
		nbAnnee=-1;
	}
	// ON VERIFIE SI L'EMPLOYE EST EMBAUCHE APRES LA DATE DU JOUR
	// SI OUI SON ANCIENNETE VAUT 0 CAR IL NE TRAVAIL PAS ENCORE
	if(anneeJ<anneeE || moisJ<moisE || jourJ<jourE)
	{
		if(anneeJ<anneeE)
		{
			nbAnnee=0;
		}
		if(anneeJ<=anneeE && moisJ<moisE)
		{
			nbAnnee=0;
		}
		if(anneeJ<=anneeE && moisJ<=moisE && jourJ<jourE)
		{
			nbAnnee=0;
		}
	}
	return nbAnnee;
}

/*
	Cette fonction sert a retirer le '\n' car nous 
	lisons le nom et prenom avec le fgets et comme 
	nous lisons la reponses dans un fgets nous devons 
	supprimer '\n' qui evitera les passages a la 
	ligne dans le fichier dat et dans le visuel
*/

int lire(char *chaine, int longueur)
{	
	// DECLARATION DES VARIABLES
    char *positionEntree = NULL;
    // ON LIT CE QUE L'UTILISATEUR METS AU CLAVIER 
	// ET ON VERIFIE SI IL EST NON NULL
    if (fgets(chaine, longueur, stdin) != NULL)  
    {
		positionEntree = strchr(chaine, '\n');
		// SI LE '/n' EST TROUVE ALORS IL SERA REMPLACE PAR '\0'
        if (positionEntree != NULL) // SI ON A TROUVE LE RETOUR A LA LIGNE
        {
            *positionEntree = '\0'; // ON LE REMPLACE PAR LE \0
        }
        return 1; // ON RENVOIE 1 SI LA FONCTION S'EST DEROULE CORRECTEMENT
	}   
    else
    {
        return 0; // ON RENVOIE 0 S'IL Y A EU UNE ERREUR
    }
}

/*
	Cette fonction sert a afficher
	la liste des statuts et d'effectuer le choix d'un status
*/

void afficherStatut(char *type)
{
	// DECLARATION DES VARIABLES
	int choix;
	
	// ON AFFICHE LE TABLEAU AVEC LES DIFFERENTS CHOIX
	printf("--------------------------\n");
	printf("| NUM  |     STATUT      |\n");
	printf("|------------------------|\n");
	printf("|   1  | DEMISSION       |\n|   2  | LICENCIE        |\n|   3  | MALADIE         |\n|   4  | PAUSE CARRIERE  |\n|   5  | RETRAITE        |\n|   6  | TRAVAIL         |\n");
	printf("--------------------------\n");
	// ON LIT LE CHOIX
	scanf("%d",&choix);
	
	// ON EFFECTUE DES CONDITIONS POUR METTRE LA VALEURS DU 
	// CHOIX DANS UNE VARIABLE
	if(choix==1)
	{
		strcpy(type,"DEMISSION");
	}
	if(choix==2)
	{
		strcpy(type,"LICENCIER");
	}
	if(choix==3)
	{
		strcpy(type,"MALADIE");
	}
	if(choix==4)
	{
		strcpy(type,"PAUSE_CARIERE");
	}
	if(choix==5)
	{
		strcpy(type,"EN_RETRAITE");
	}
	if(choix==6)
	{
		strcpy(type,"TRAVAIL");
	}
}

/*
	Cette fonction sert a convertir une chaines de caracteres en majuscule
*/

void convertirMaju(char *chaine)
{
	// DECLARATION DES VARIABLES
	int i;
	
	// BOUCLE DE 0 JUSQU'A LA LONGUEUR DE CHAINE
	for (i=0; i<strlen(chaine); i++)
	{
		// CONVERTIT LE CARACTERE[i] EN MAJUSCULE GRACE AU TOUPPER
    	chaine[i]=toupper(chaine[i]);
	}
}

/*
	Cette fonction sert a calculer le salaire de chaque employe en net
	grace au nombres d'heures semaine, au tarif horaire et à l'indexation
*/

float calculSalaire(float tarif, float index, Employe *courant)
{
	// DECLARATION DES VARIABLES
	int i, age;
	float salaire = (tarif*(float)courant->heureSem)*4.;

	// CONVERTI L'INDEX EN 1.xx POUR POUVOIR CALCULER LE SALAIRE
	index=(index/100)+1;

	time_t now;
	// RENVOIE L'HEURE ACTUELLE
	time(&now);
	// L'HEURE EST CONVERTI A L'HEURE LOCAL
	struct tm *local = localtime(&now);   
	int jourJ = local->tm_mday;          
	int moisJ = local->tm_mon + 1;     
	int anneeJ = local->tm_year + 1900; 
		
	age=anneeJ-courant->anneeN;

	// ON VERIFIE SI L'EMPLOYE EST EMBAUCHE APRES LA DATE DU JOUR
	// SI OUI SON SALAIRE VAUT 0 CAR IL NE TRAVAIL PAS ENCORE
	if(anneeJ<courant->anneeE || moisJ<courant->moisE || jourJ<courant->jourE)
	{
		if(anneeJ<courant->anneeE)
		{
			salaire=0.;
		}
		if(anneeJ<=courant->anneeE && moisJ<courant->moisE)
		{
			salaire=0.;
		}
		if(anneeJ<=courant->anneeE && moisJ<=courant->moisE && jourJ<courant->jourE)
		{
			salaire=0.;
		}
	}

	if(strcmp("STAGIAIRE", courant->type)==0 || strcmp("DEMISSION", courant->statut)==0 || strcmp("LICENCIER", courant->statut)==0 || strcmp("PAUSE-CARIERE", courant->statut)==0 || strcmp("EN_RETRAITE", courant->statut)==0)
	{
		salaire=0;
	}
	
	//VERIFIE SI L'EMPLOYE EST UN ETUDIANT CAR LE SALAIRE SERA
	//CALCULER SELON SON AGE AVEC UN CERTAIN %
	else if(strcmp("ETUDIANT", courant->type)==0)
	{
		if(age<=16)
		{
			salaire=salaire*0.7;
		}
		else if(age==17)
		{
			salaire=salaire*0.76;
		}
		else if(age==18)
		{
			salaire=salaire*0.82;
		}
		else if(age==19)
		{
			salaire=salaire*0.88;
		}
		else if(age==20)
		{
			salaire=salaire*0.94;
		}
	}
	// SI IL NE S'AGIT PAS D'UN ETUDIANT ON CALCULERA LE SALAIRE AVEC LE NOMBRE D'ANNEE D'ANCIENNETE
	// AVEC L'AJOUT DE L'INDEXATION
	else
	{
		for(i=2;i<=courant->anciennete;i++)
		{
			salaire=salaire*index;
		}
		if(strcmp("EMPLOYE", courant->type)==0)
		{
			salaire=salaire*1,02;
		}
		else if(strcmp("CADRE", courant->type)==0)
		{
			salaire=salaire*1,05;
		}
		// UN STAGIAIRE NE GAGNE PAS D'ARGENT PAR LA LOI BELGE
		else if(strcmp("STAGIAIRE", courant->type)==0)
		{
			salaire=0.;
		}
	}

	return salaire;
}

/*
	Cette fonction sert a savoir le type de contract qu'a l'employé,
	temps partiel ou temps plein
*/

void typeContract(Employe *courant)
{
	//SI LE NOMBRE D'HEURE SEMAINE EST EGALE A 38 ->TEMPS PLEIN
	//NE SERA PAS SUPERIEUR A 38 CAR EN BELGIQUE ON NE PEUT PAS
	//TRAVAILLER PLUS DE 38 HEURES PAR SEMAINE
	if(courant->heureSem==38)
	{
		strcpy(courant->typeContrat, "TEMPS PLEIN");	
	}
	else
	{
		strcpy(courant->typeContrat, "TEMPS PARTIEL");
	}
}

/*
	Cette fonction sert à afficher la liste de tout les employés
*/

void afficherListe(Employe *de, int nb)
{
	// DECLARATION DES VARIABLES
	int i;
	
	// AFFICHAGE DES TITRES
	printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|                                                                                                    LISTE DES EMPLOYES                                                                                                   |\n");
	printf("|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
	printf("| NUM  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | DATE EMBA. |      TYPE       |          SERVICE           | ANCIENNETE | HEURES/SEM |    CONTRAT    |      SALAIRE      | SEXE |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+------------+-----------------+----------------------------+------------+------------+---------------+--------------------------|\n");
	
	Employe *cour;
	cour=de;
	
	// AFFICHAGE DES EMPLOYES
	for(i=1;i<=nb;i++)
	{
		printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %02d/%02d/%4d | %-15s | %-26s |   %02d ans   |     %2dh    | %-13s | %8.2fEuro net  |   %1s  |\n", 
		i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
		cour->anneeE, cour->type, cour->fonction, cour->anciennete, cour->heureSem, cour->typeContrat, cour->salaire, cour->sexe);
		cour=cour->suivant;
	}
	printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	// AFFICHAGE DU NOMBRES D'EMPLOYES
	printf("| Nombre d'employes : %d employe(s)\n", nb);
}

/*
	Cette fonction sert à afficher la liste de tout les 
	services present dans la société avec 
	leurs tarifs horaires et leurs indexation
*/

void afficherListeService(Service *debSer, int nb)
{
	// DECLARATION DES VARIABLES
	int i;
	
	// AFFICHAGE DES TITRES
	printf("------------------------------------------------------------------------\n");
	printf("| NUM  |           SERVICE            | TARIF/HORAIRE |  INDEXATION/AN |\n");
	printf("|------+------------------------------+---------------+----------------|\n");
	
	Service *courSer;
	courSer=debSer;
	
	// AFFICHAGE DES SERVICES
	for(i=1;i<=nb;i++)
	{
		printf("|  %02d  | %-28s | %5.2f e/h     | %5.2f%%         |\n", 
		i, courSer->nom, courSer->tarifHoraire, courSer->indexation);
		courSer=courSer->suivant;
	}
	printf("------------------------------------------------------------------------\n");
	// AFFICHAGE DU NOMBRES DE SERVICES
	printf("Nombre de service : %d service(s)\n", nb);
}

/*
	Cette fonction sert a enregistrer et donc ecrire les données dans le fichier dat
*/

void enregistrerListe(Employe *cour, int nb, Service *courantSer, int nbService)
{
	// OUVERTURE DES FICHIER NECESSAIRES POUR L'ENREGISTREMENT 
	// -> "ManzanoGodemont.dat" & "ManzanoGodemont02.dat"  ECRIRE LES LISTES CHAINEES 
	FILE *fres, *fres2;
	fres=fopen("ManzanoGodemont.dat","w");
	fres2=fopen("ManzanoGodemont02.dat","w");
	
	// DECLARATIONS DES VARIABLES
	int i;
	
	// ECRITURE DES RESULTATS DANS LES FICHIERS
	for(i=1;i<=nb;i++)
	{
		fprintf(fres, "%-8s%-26s%-26s %02d/%02d/%4d %02d/%02d/%4d %-10s %-25s %-14s %-1s %2d\n", 
		cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
		cour->anneeE, cour->type, cour->fonction, cour->statut,cour->sexe,cour->heureSem);
		cour=cour->suivant;
	}
	for(i=1;i<=nbService;i++)
	{
		fprintf(fres2, "%5.2f %-26s %5.2f\n", courantSer->tarifHoraire, courantSer->nom, courantSer->indexation);
		courantSer=courantSer->suivant;
	}
}

/*
	Cette fonction sert a choisir un type pour chaque employe
*/

void choixType(char *type)
{
	// DECLARATIONS DES VARIABLES
	int choix;
	
	// AFFICHAGE DES TITRES ET DES CHOIX DES DIFFERENTS TYPE D'EMPLOYES
	printf("--------------------------\n");
	printf("| NUM  |       NOM       |\n");
	printf("|------------------------|\n");
	printf("|   1  | CADRE           |\n|   2  | EMPLOYE         |\n|   3  | OUVRIER         |\n|   4  | TEMPORAIRE      |\n|   5  | DEFINITIF       |\n|   6  | ETUDIANT        |\n|   7  | STAGIAIRE       |\n");
	printf("--------------------------\n");
	// ON LIT LE CHOIX
	scanf("%d",&choix);
	
	// ON EFFECTUE DES CONDITIONS POUR METTRE LA VALEURS DU 
	// CHOIX DANS UNE VARIABLE
	if(choix==1)
	{
		strcpy(type,"CADRE");
	}
	else if(choix==2)
	{
		strcpy(type,"EMPLOYE");
	}
	else if(choix==3)
	{
		strcpy(type,"OUVRIER");
	}
	else if(choix==4)
	{
		strcpy(type,"TEMPORAIRE");
	}
	else if(choix==5)
	{
		strcpy(type,"DEFINITIF");
	}
	else if(choix==6)
	{
		strcpy(type,"ETUDIANT");
	}
	else if(choix==7)
	{
		strcpy(type,"STAGIAIRE");
	}
}

/*
	Cette fonction sert a chaque ajout d'employé
	de choisir son service
*/

void choixService(Service *debSer, int nbService, Employe *courant)
{	
	// DECLARATIONS DES VARIABLES
	int i, choix;
	
	Service *courantSer;
	
	// ON DEMANDE A L'UTILISATEUR DE CHOISIR LE SERVICE VOULU
	printf("Quel est le service : \n");
	courantSer=debSer;
	// ON AFFICHE LA LISTE DES SERVICES
	afficherListeService(debSer, nbService);
	scanf("%d",&choix);
		
	courantSer=debSer;
	for(i=1;i<=nbService;i++)
	{
		if(i==choix)
		{
			strcpy(courant->fonction,courantSer->nom);
			break;
		}
		courantSer=courantSer->suivant;
	}
}

/*
	Cette fonction sert a ajouter un d'employé
*/

void ajouterEmploye(Employe **deb, int *nb, Service *debSer, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int i,choixServ,j=0,nb_alea = rand()%100+1;
	char statuts[13],buffer[3];
	
	// ON AFFICHE LA LISTE DES EMPLOYES 
	afficherListe(*deb, *nb);
	Employe *intercale;
	Service *courantSer;
	intercale=malloc(sizeof(Employe));
	intercale->suivant=*deb;
	*deb=intercale;
	
	time_t now;
    	
	// AUGMENTER COMPTEUR DE PERSONNE DANS LA LISTE CHAINEE
	*nb=*nb+1;
	
	srand ( time(NULL) );

	printf("\n\nDemande d'information\n");
	printf("---------------------\n");
	
	// LECTURE DU NOM DU NOUVEL EMPLOYE
	lire(intercale->nom, 25);
    printf("\nQuel est votre nom ?\n");
    lire(intercale->nom, 25);
    
    // ON CONVERTIT LE NOM EN MAJUSCULE
    convertirMaju(intercale->nom);
    
    // LECTURE DU PRENOM DU NOUVEL EMPLOYE
    printf("Quel est votre prenom ?\n");
    lire(intercale->prenom, 25);
    
    // ON CONVERTIT LE PRENOM EN MAJUSCULE
    convertirMaju(intercale->prenom);
    
	// ON CONVERTIT LE NUMERO ALEATOIRE EN CHAINE DE CARACTERE
	sprintf(buffer, "%d", nb_alea);
	
    // ON DEFINIT LE NUMERO ALEATOIRE APRES LA LECTURE DU NOM ET DU PRENOM
    // LE NUMERO ALEATOIRES EST COMPOSE D'UN NOMBRES ALEATOIRES DE 2 CHIFFRES,
	// LES 3 PREMIERES LETTRES DU NOM ET DES 3 PREMIERES LETTRES DU PRENOM
	for(i=0;i<2;i++)
	{
		intercale->num[j]=buffer[i];
		j++;
	}
	for(i=0;i<3;i++)
	{
		intercale->num[j]=intercale->nom[i];
		j++;
	}
	for(i=0;i<3;i++)
	{
		intercale->num[j]=intercale->prenom[i];
		j++;
	}
	// ON ATTRIBUE A LA DERNIERE CASE DE LA CHAINE DE CARACTERES UN \0
	// POUR DEFINIR LA FIN DE LA CHAINE DE CARACTERE
	intercale->num[j]='\0';
	
	// LECTURE DE LA DATE DE NAISSANCE DU NOUVEL EMPLOYE
    printf("Quelle est la date de naissance (JJ/MM/AAAA) ?\n");
    scanf("%d %*c %d %*c %d", &intercale->jourN, &intercale->moisN, &intercale->anneeN);
    // ON REDEMANDE LA DATE DE NAISSANCE TANT QU'ELLE NE RESPECTE PAS LES CONDITIONS
    while(intercale->jourN <1 || intercale->jourN >31 || intercale->moisN <1 || intercale->moisN>12 || intercale->anneeN<1900 || intercale->anneeN>2500)
    {
    	printf("ERREUR DE DATE -> Introduisez une date correcte de type JJ/MM/AAAA !\n");
    	scanf("%d %*c %d %*c %d", &intercale->jourN, &intercale->moisN, &intercale->anneeN);
	}
    // LECTURE DE LA DATE D'EMBAUCHE DU NOUVEL EMPLOYE
    printf("Quelle est la date d'embauche (JJ/MM/AAAA) ?\n");
    scanf("%d %*c %d %*c %d", &intercale->jourE, &intercale->moisE, &intercale->anneeE);
    // ON REDEMANDE LA DATE D'EMBAUCHE TANT QU'ELLE NE RESPECTE PAS LES CONDITIONS
    while(intercale->jourE <1 || intercale->jourE >31 || intercale->moisE <1 || intercale->moisE>12 || intercale->anneeE<1900 || intercale->anneeE>2500)
    {
    	printf("ERREUR DE DATE -> Introduisez une date correcte de type JJ/MM/AAAA !\n");
    	scanf("%d %*c %d %*c %d", &intercale->jourE, &intercale->moisE, &intercale->anneeE);
	}
	
	// LECTURE DU TYPE DU NOUVEL EMPLOYE
	printf("De quel type d'employe s'agit-il ?\n");
	choixType(intercale->type);
	
	// LECTURE DU SERVICE DU NOUVEL EMPLOYE
	choixService(debSer,nbService,intercale);
	
	// LECTURE DU NOMBRES D'HEURES PAR SEMAINES DU NOUVEL EMPLOYE
	printf("Quel est le nombre d'heures par semaine ?\n");
    scanf("%d", &intercale->heureSem);

	// ON APPEL LA FONCTION POUR CALCULER L'ANCIENNETE
	intercale->anciennete=calculerAnciente(intercale->jourE, intercale->moisE, intercale->anneeE);
	
	courantSer=debSer;
	for(i=1;i<=nbService;i++)
	{
		if(strcmp(intercale->fonction, courantSer->nom)==0)
		{
			break;
		}
		courantSer=courantSer->suivant;
	}
	
	// LECTURE DU STATUT DU NOUVEL EMPLOYE
	printf("Quel est le statut?\n");
	afficherStatut(intercale->statut);
	
	// ON APPEL LA FONCTION POUR DEFINIR LE TYPE DE CONTRAT
	typeContract(intercale);
	
	START:
	// ON DEMANDE LE SEXE DE L'EMPLOYE
	printf("Quel est le sexe ? \n1 : Homme\n2 : Femme\n");
	scanf("%1d",&i);
	
	if(i==1)
	{
		strcpy(intercale->sexe,"M");
	}
	else if(i==2)
	{
		strcpy(intercale->sexe,"F");
	}
	else 
	{
		printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
		goto START;
	}
	// ON APPEL LA FONCTION POUR CALCULER LE SALAIRE
	intercale->salaire=calculSalaire(courantSer->tarifHoraire,courantSer->indexation, &*intercale);
	
	system(clear);
}

/*
	Cette fonction sert a ajouter un service
	à la liste des services existante
*/

void ajouterService(Service **debSer, int *nbService)
{
	// DECLARATIONS DES VARIABLES
	float reponses;
	
	afficherListeService(*debSer, *nbService);
	
	// AUGMENTER COMPTEUR DE PERSONNE DANS LA LISTE CHAINEE
	*nbService=*nbService+1;
	
	Service *intercaleSer;
	
	intercaleSer=malloc(sizeof(Service));
	intercaleSer->suivant=*debSer;
	*debSer=intercaleSer;
	
	// LECTURE DU NOM DU NOUVEAU SERVICE
	lire(intercaleSer->nom, 25);
    printf("\nQuel est le nouveau Service ?\n");
    lire(intercaleSer->nom, 25);
    
    // ON CONVERTIT LE NOM DU SERVICE EN MAJUSCULE
    convertirMaju(intercaleSer->nom);
    
    // LECTURE DU TAUX D'INDEXATION DU NOUVEAU SERVICE
    printf("\nQuel est le taux d'indexation en pourcent (XX%%) ?\n");
    scanf("%f", &reponses);
    intercaleSer->indexation=reponses;
    
    // LECTURE DU SALAIRE HORAIRE DU NOUVEAU SERVICE
    printf("\nQuel est le salaire horaire en euros ?\n");
    scanf("%f", &reponses);
    intercaleSer->tarifHoraire=reponses;
	system(clear);
}

/*
	Cette fonction sert a supprimer un employé de la liste des employes
*/

void supprimerEmploye(Employe **deb, int *nb)
{
	// DECLARATIONS DES VARIABLES
	int pos, i;
	char reponse[1];

	Employe *intercale, *courant;
	
	START:
	
	// ON AFFICHE LA LISTE DES EMPLOYES
	afficherListe(*deb, *nb);
	
	printf("Etes-vous sures de vouloir supprimer un employe ? (o/n)\n");
	scanf("%s",&reponse);
	if(strcmp(reponse,"o")==0)
	{
		// ON DEMANDE EN QUELLE POSITION IL FAUT SUPPRIMER L'EMPLOYE
		// ON SUPPRIME L'EMPLOYE
		printf("Supprimer un employe en quelle position ?\n");
		scanf("%d", &pos);
		if(pos>=1 && pos<=*nb)
		{
			if(pos==1)
			{
				intercale=*deb;
				*deb=intercale->suivant;
				free(intercale);
			}
			else
			{
				courant=*deb;
				for(i=1;i<pos-1;i++)
				{
					courant=courant->suivant;
				}
				if(pos!=*nb)
				{
					intercale=courant->suivant;
					courant->suivant=intercale->suivant;
					free(intercale);
				}
				else
				{
					intercale=courant->suivant;
					courant->suivant=NULL;
					free(intercale);
				}
			}
		}
		else
		{
			// SI L'UTILISATEUR ENCODE UNE VALEURS ERRONE, ON LUI INDIQUE ET ON RETOURN AU START
				system(clear);
				printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
				system("PAUSE");
				system(clear);
				goto START;
		}
		*nb=*nb-1;
	}
	system(clear);
}

/*
	Cette fonction sert a supprimer un service de la liste des services
*/

void supprimerService(Service **debSer, int *nb)
{
	// DECLARATIONS DES VARIABLES
	int pos, i;
	char reponse[1];
	
	Service *intercaleSer, *courantSer;
	
	START:
		
	// ON AFFICHE LA LISTE DES SERVICES
	afficherListeService(*debSer, *nb);
	
	// ON DEMANDE EN QUELLE POSITION IL FAUT SUPPRIMER LE SERVICE
	// ON SUPPRIME LE SERVICE
	printf("Etes-vous sures de vouloir supprimer un service ? (o/n)\n");
	scanf("%s",&reponse);
	if(strcmp(reponse,"o")==0)
	{
		printf("Supprimer un service en quelle position ?\n");
		scanf("%d", &pos);
		if(pos>=1 && pos<=*nb)
		{
			if(pos==1)
			{
				intercaleSer=*debSer;
				*debSer=intercaleSer->suivant;
				free(intercaleSer);
			}
			else
			{
				courantSer=*debSer;
				for(i=1;i<pos-1;i++)
				{
					courantSer=courantSer->suivant;
				}
				if(pos!=*nb)
				{
					intercaleSer=courantSer->suivant;
					courantSer->suivant=intercaleSer->suivant;
					free(intercaleSer);
				}
				else
				{
					intercaleSer=courantSer->suivant;
					courantSer->suivant=NULL;
					free(intercaleSer);
				}
			}
			printf("Le service a correctement ete supprime\n");
			system("PAUSE");
		}
		else
		{
			// SI L'UTILISATEUR ENCODE UNE VALEURS ERRONE, ON LUI INDIQUE ET ON RETOURN AU START
			system(clear);
			printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
			system("PAUSE");
			system(clear);
			goto START;
		}
		*nb=*nb-1;
	}
	system(clear);
}

/*
	Cette fonction sert a afficher les anniversaires
	du mois en court
*/

void anniversaireMois(Employe *de, int nb)
{
	system(clear);
	
	// DECLARATIONS DES VARIABLES
	int moisJ, i, anneeJ, age, nbAnn=0;
	
	Employe *cour;
	cour=de;
	
  	time_t now;
    
	// RENVOIE L'HEURE ACTUEL
	time(&now);
	// L'HEURE EST CONVERTI A L'HEURE LOCAL
	
	struct tm *local = localtime(&now);     
	moisJ = local->tm_mon + 1;
	
	// AFFICHAGE DES TITRES ET AFFICHAGE DES ANNIVERSAIRES DANS LE MOIS EN COURS
	printf("------------------------------------------------------------------------------------------------\n");
	printf("| NUM  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | AGE |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+-----|\n");
	
	for(i=1;i<=nb;i++)
	{
		if(cour->moisN==moisJ)
		{
			nbAnn++;
			anneeJ = local->tm_year + 1900; 
			age=anneeJ-cour->anneeN;
			printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %2d  |\n", 
			i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, age);
			
		}
		cour=cour->suivant;
	}
	// SI IL N'Y A PAS D'ANNIVERSAIRES ON AFFICHE QU'IL N'Y EN A AUCUN
	if(nbAnn==0)
	{
		printf("\n 			Il y a aucun anniversaire ce mois-ci!\n\n");
	}
	printf("------------------------------------------------------------------------------------------------\n");
	// ON AFFICHE LE NOMBRES TOTAL D'ANNIVERSAIRES DANS LE MOIS EN COURS
	printf("Nombre d'anniversaires ce mois-ci : %d anniversaire(s)\n\n", nbAnn);
}

/*
	Cette fonction sert a modifier un employé
*/

void modifierEmploye(Employe *deb, int nb, Service *debSer, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int pos, i,choixServ;
	char reponse[1];
	int lire(char *, int );
	
	Employe *intercale, *courant;
	Service *courantSer;
	
	// ON AFFICHE LA LISTE DES EMPLOYES
	afficherListe(deb, nb);
	
	// ON DEMANDE EN QUEL POSITION MODIFIER UN EMPLOYE
	printf("\n\nModifier un employe en quelle position ?\n");
	scanf("%d", &pos);
	courant=deb;
	if(pos>=1 && pos<=nb)
	{
		for(i=1;i<pos;i++)
		{
			courant=courant->suivant;
		}
		// ON DEMANDE LE NOUVEAU NOM ET ON LE LIT
		// ET ON CONVERTIT EN MAJUSCULE
		printf("Voulez-vous modifier le nom ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			//lire(courant->nom, 25);
			printf("Quel est le nouveau nom : ");
			lire(courant->nom, 25);
			lire(courant->nom, 25);
			convertirMaju(courant->nom);
		}
		// ON DEMANDE LE NOUVEAU PRENOM ET ON LE LIT
		// ET ON CONVERTIT EN MAJUSCULE
		printf("Voulez-vous modifier le prenom ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("Quel est le nouveau prenom : ");
			lire(courant->prenom, 25);
			lire(courant->prenom, 25);
			convertirMaju(courant->prenom);
		}
		// ON DEMANDE LA NOUVEL DATE DE NAISSANCE ET ON LA LIT
		printf("Voulez-vous modifier la date de naissance ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("Quelle est la date de naissance : ");
			scanf("%d %*c %d %*c %d", &courant->jourN, &courant->moisN, &courant->anneeN);
			// ON REDEMANDE LA DATE DE NAISSANCE TANT QU'ELLE NE RESPECTE PAS LES CONDITIONS
			while(courant->jourN <1 || courant->jourN >31 || courant->moisN <1 || courant->moisN>12 || courant->anneeN<1900 || courant->anneeN>2500)
		    {
		    	printf("ERREUR DE DATE -> Introduisez une date correcte de type JJ/MM/AAAA !\n");
		    	scanf("%d %*c %d %*c %d", &courant->jourN, &courant->moisN, &courant->anneeN);
			}
		}
		// ON DEMANDE LA NOUVEL DATE D'EMBAUCHE ET ON LA LIT
		printf("Voulez-vous modifier la date d'embauche ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("Quelle est la date d'embauche : ");
			scanf("%d %*c %d %*c %d", &courant->jourE, &courant->moisE, &courant->anneeE);
			// ON REDEMANDE LA DATE D'EMBAUCHE TANT QU'ELLE NE RESPECTE PAS LES CONDITIONS
		    while(courant->jourE <1 || courant->jourE >31 || courant->moisE <1 || courant->moisE>12 || courant->anneeE<1900 || courant->anneeE>2500)
		    {
		    	printf("ERREUR DE DATE -> Introduisez une date correcte de type JJ/MM/AAAA !\n");
		    	scanf("%d %*c %d %*c %d", &courant->jourE, &courant->moisE, &courant->anneeE);
			}
			// ON APPEL LA FONCTION POUR CALCULER L'ANCIENNETE
			courant->anciennete=calculerAnciente(courant->jourE, courant->moisE, courant->anneeE);
			courantSer=debSer;
			for(i=1;i<=nbService;i++)
			{
				if(strcmp(courant->fonction, courantSer->nom)==0)
				{
					break;
				}
				courantSer=courantSer->suivant;
			}
			// ON APPEL LA FONCTION POUR CALCULER LE SALAIRE
			courant->salaire=calculSalaire(courantSer->tarifHoraire,courantSer->indexation, &*courant);
		}
		// ON DEMANDE LE NOUVEAU TYPE DE L'EMPLOYE ET ON LE LIT
		printf("Voulez-vous modifier le type d'employe ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			choixType(courant->type);
		}
		// ON DEMANDE LE NOUVEAU SERVICE DE L'EMPLOYE ET ON LE LIT
		printf("Voulez-vous modifier le service ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			choixService(debSer,nbService,courant);
		}	
		// ON DEMANDE LE NOUVEAU NOMBRE D'HEURES DE L'EMPLOYE ET ON LE LIT
		printf("Voulez-vous modifier le nombre d'heures par semaine ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("Quel est le nombre d'heures semaine : ");
			scanf("%d",&courant->heureSem);
			courantSer=debSer;
			for(i=1;i<=nbService;i++)
			{
				if(strcmp(courant->fonction, courantSer->nom)==0)
				{
					break;
				}
				courantSer=courantSer->suivant;
			}
			// ON APPEL LA FONCTION POUR CALCULER LE SALAIRE
			courant->salaire=calculSalaire(courantSer->tarifHoraire,courantSer->indexation, &*courant);
			
			// ON APPEL LA FONCTION POUR DEFINIR LE TYPE DE CONTRAT
			typeContract(courant);
		}
	}
	system(clear);
}

/*
	Cette fonction sert a modifier le service
*/

void modifierService(Service *courantSer, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int pos,i;
	char reponse[1];
	float reponses;
	
	// ON AFICHE LA LISTE DES SERVICES
	afficherListeService(courantSer, nbService);

	// ON DEMANDE EN QUELLE POSITION SE TROUVE LE SERVICE A MODIFIER
	printf("\n\nModifier un service en quelle position ?\n");
	scanf("%d", &pos);
	if(pos>=1 && pos<=nbService)
	{
		for(i=1;i<pos;i++)
		{
			courantSer=courantSer->suivant;
		}
		
		// ON DEMANDE LE NOUVEAU TAUX D'INDEXATION DU SERVICE ET ON LE LIT
		printf("Voulez-vous modifier le taux d'indexation en pourcent (XX%%) ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("\nQuel est le taux d'indexation en pourcent (XX%%) ?\n");
	    	scanf("%f", &reponses);
	    	courantSer->indexation=reponses;
		}
		
		// ON DEMANDE LE NOUVEAU TARIF HORAIRE DU SERVICE ET ON LE LIT
		printf("Voulez-vous modifier le salaire horaire en euros ? (o/n)\n");
		scanf("%s",&reponse);
		if(strcmp(reponse,"o")==0)
		{
			printf("\nQuel est le salaire horaire en euros ?\n");
	    	scanf("%f", &reponses);
	    	courantSer->tarifHoraire=reponses;
		}
	}
	system(clear);
}

/*
	Cette fonction sert a afficher les employé 
	selon un service choisi
*/

void afficherEmpServ(Employe *de, int nb, Service *debServ, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int choix,i, n=0;
	char fonctions[13],choix2[1];
	
	Employe *cour;
	Service *courantSer;
	courantSer=debServ;
		
	// ON AFFICHE LA LISTE DES SERVICES
	// ET ON DEMANDE IL FAUT AFFICHER LES EMPLOYES DE QUEL SERVICES
	printf("Quel est le service : \n");
	afficherListeService(debServ, nbService);
	scanf("%d",&choix);

	courantSer=debServ;
	for(i=1;i<=nbService;i++)
	{
		if(i==choix)
		{
			strcpy(fonctions,courantSer->nom);
			break;
		}
		courantSer=courantSer->suivant;
	}	
	system(clear);
	
	// AFFICHAGE DES TITRES
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|                                                                         LISTE DES EMPLOYES DU SERVICE %-28s                                                    |\n", fonctions);
	printf("|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("| NUM  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | DATE EMBA. |       TYPE      |          SERVICE           | ANCIENNETE |       SALAIRE       |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+------------+-----------------+----------------------------+------------+---------------------+\n");
	
	cour=de;
	// ON AFFICHE LA LISTES DES EMPLOYES SELON LE SERVICE CHOISI
	for(i=1;i<=nb;i++)
	{
		if(strcmp(fonctions, cour->fonction)==0)
		{
			printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %02d/%02d/%4d | %-15s | %-22s |   %02d ans   |  %8.2fEuro brut  |\n", 
			i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
			cour->anneeE, cour->type, cour->fonction, cour->anciennete, cour->salaire);
			n++;
		}
		cour=cour->suivant;
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	// ON AFFICHE LE NOMBRE TOTAL D'EMPLOYES SELON LE SERVICE CHOISI
	printf("| Nombre d'employe : %d employe(s) dans le service %s\n\n\n\n\n", n, fonctions);
}

/*
	Cette fonction sert a affiché les employés
	selon son statut choisi par l'utilisateur
*/

void afficherEmpStatut(Employe *de, int nb)
{
	// DECLARATIONS DES VARIABLES
	int i, n=0;
	char statuts[13];
	
	Employe *cour;
	
	printf("Quel est le statut : \n");
	
	// ON AFFICHE LA LISTE DES STATUTS
	afficherStatut(statuts);
	
	system(clear);
	
	// AFFICHAGE DES TITRES
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|                                                                   LISTE DES EMPLOYES AYANT POUR STATUT : %-28s                                                                   |\n", statuts);
	printf("|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
	printf("| NUM  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | DATE EMBA. |      TYPE       |          SERVICE           | ANCIENNETE |       SALAIRE       |      STATUT     |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+------------+-----------------+----------------------------+------------+---------------------+-----------------|\n");
	
	cour=de;
	// ON AFFICHE LA LISTES DES EMPLOYES SELON LE STATUT CHOISI
	for(i=1;i<=nb;i++)
	{
		if(strcmp(statuts, cour->statut)==0)
		{
			printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %02d/%02d/%4d | %-15s | %-22s |   %02d ans   |  %8.2fEuro brut  | %-15s |\n", 
			i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
			cour->anneeE, cour->type, cour->fonction, cour->anciennete, cour->salaire, cour->statut);
			n++;
		}
		cour=cour->suivant;
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	// ON AFFICHE LE NOMBRE TOTAL D'EMPLOYES SELON LE STATUT CHOISI
	printf("| Nombre d'employe : %d employe(s) avec le statut %s\n\n\n\n\n", n, statuts);
}

/*
	Cette fonction sert a modifier le statut de l'employé
*/

void modifierStatutEmp(Employe **deb, int *nb)
{
	// DECLARATIONS DES VARIABLES
	int i,pos;

	Employe *cour;
	
	system(clear);
	
	// AFFICHAGE DES TITRES
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|                                                                                      LISTE DES EMPLOYES LEUR STATUT                                                                                     |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("| NUM  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | DATE EMBA. |      TYPE       |          SERVICE           | ANCIENNETE |       SALAIRE       |      STATUT     |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+------------+-----------------+----------------------------+------------+---------------------+-----------------|\n");
	
	cour=*deb;
	// ON AFFICHE LA LISTES DES EMPLOYES AVEC LEURS STATUT
	for(i=1;i<=*nb;i++)
	{
		
			printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %02d/%02d/%4d | %-15s | %-22s |   %02d ans   |  %8.2fEuro brut  | %-15s |\n", 
			i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
			cour->anneeE, cour->type, cour->fonction, cour->anciennete, cour->salaire, cour->statut);
			cour=cour->suivant;
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("Le statut de quel employe voulez-vous modifier ? \n");
	
	cour=*deb;
	// ON DEMANDE EN QUELLE POSITION SE TROUVE L'EMPLOYE A MODIFIER
	scanf("%d", &pos);
	if(pos>=1 && pos<=*nb)
	{
		for(i=1;i<pos;i++)
		{
			cour=cour->suivant;
		}
		// ON AFFICHE LA LISTE DES STATUTS
		afficherStatut(cour->statut);
	}
	system(clear);
}

/*
	Cette fonction permet d'effectuer une backup des fichiers de données
*/

void backup(Employe *deb,int nbEmploye, Service *debSer, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int i;
	char nomFichier[256], choix[1];
	
	// ON DEMANDE SI L'UTILISATEUR VEUT VRAIMENT EFFECTUER UNE BACKUP
	printf("Voulez-vous vraiment effectuer un backup des fichiers ? (o/n)\n");
	scanf("%s",choix);
	if(strcmp(choix,"o")==0)
	{
		START:
		// ON LIT LE NOM DU FICHIER A ATTRIBUER A LA BACKUP DE LA LISTE DES EMPLOYES
		printf("Quel est le nom du backup pour les employes ? (Ne pas oublie l'extension .dat , .res ,...)\n");
		lire(nomFichier, 256);
		lire(nomFichier, 256);
		
		// ON VERIFIE QUE LA CHAINE DE CARACTERE NE DEPASSE PAS 256 CARACTERES
		if(strlen(nomFichier)>254)
		{
			printf("ERREUR -> NOM DE FICHIER TROP LONG\n");
			goto START;
		}
		
		// ON ATTRIBUE LA VALEURS ENCODER AU NOM DU FICHIERS
		FILE *fresBE;
		fresBE=fopen(nomFichier,"w");
		// ON ECRIT LES RESULTAT DANS LE FICHIER DE BACKUP 1
		for(i=1;i<=nbEmploye;i++)
		{
			fprintf(fresBE, "%-8s%-26s%-26s %02d/%02d/%4d %02d/%02d/%4d %-11s %-25s %-14s %-1s %2d\n", 
			deb->num, deb->nom, deb->prenom, deb->jourN, deb->moisN, deb->anneeN, deb->jourE, deb->moisE, 
			deb->anneeE, deb->type, deb->fonction, deb->statut,deb->sexe,deb->heureSem);
			deb=deb->suivant;
		}
		// ON FERME LE FICHIERS DE BACKUP 1
		fclose(fresBE);
	
		START2:
		// ON LIT LE NOM DU FICHIER A ATTRIBUER A LA BACKUP DE LA LISTE DES SERVICES
		printf("Quel est le nom du backup pour les services ? (Ne pas oublie l'extension .dat , .res ,...)\n");
		lire(nomFichier, 256);
		
		// ON VERIFIE QUE LA CHAINE DE CARACTERE NE DEPASSE PAS 256 CARACTERES
		if(strlen(nomFichier)>254)
		{
			printf("ERREUR -> NOM DE FICHIER TROP LONG\n");
			goto START2;
		}
		
		// ON ATTRIBUE LA VALEURS ENCODER AU NOM DU FICHIERS
		FILE *fresBS;
		fresBS=fopen(nomFichier,"w");
		// ON ECRIT LES RESULTAT DANS LE FICHIER DE BACKUP 2
		for(i=1;i<=nbService;i++)
		{
			fprintf(fresBS, "%5.2f %-26s %5.2f\n", debSer->tarifHoraire, debSer->nom, debSer->indexation);
			debSer=debSer->suivant;
		}
		// ON FERME LE FICHIERS DE BACKUP 2
		fclose(fresBS);
		
		printf("\nLES BACKUPS DES FICHIERS ONT CORRECTEMENT ETE EFFECTUES\n");
		system("PAUSE");
		}
}

/*
	Cette fonction permet d'exporter les données dans un fichier csv
*/

void exporterCSV(Employe *deb,int nbEmploye, Service *debSer, int nbService)
{
	// DECLARATIONS DES VARIABLES
	int i, moisJ, anneeJ;
	char nomFichiers[]="ListeEmployes",mois[50],annee[5], choix[1];
	char nomFichiers2[]="ListeServices";
	
	// ON DEMANDE SI L'UTILISATEUR VEUT VRAIMENT ECPORTER LES FICHIERS AU FORMATS CSV
	printf("Voulez-vous vraiment exporter les fichiers au format CSV ? (o/n)\n");
	scanf("%s",choix);
	if(strcmp(choix,"o")==0)
	{
		time_t now;
	    
		// RENVOIE L'HEURE ACTUEL
		time(&now);
		// CONVERTI A L'HEURE LOCAL
		struct tm *local = localtime(&now);          
		moisJ = local->tm_mon + 1;     
		anneeJ = local->tm_year + 1900; 
		
		// ON CONCATENE LE MOIS, L'ANNEE ET LE NOM DU FICHIERS POUR DEFINIR LE NOUVEAU NOM DU FICHIER
		sprintf(mois, "%d", moisJ);
		sprintf(annee, "%d", anneeJ);
		strcat(mois,"_");
		strcat(mois,annee);
		strcat(mois,"_");
		strcat(mois,nomFichiers);
		strcat(mois,".csv");
		
		// ON DECLARE ET ON OUVRE LE FICHIER AU FORMATS CSV
		FILE *fresBE;
		fresBE=fopen(mois,"w");
		
		// AFFICHAGE DES TITRES
		fprintf(fresBE,"ID;NOM;PRENOM;SEXE;DATE NAIS.;DATE EMBA.;TYPE;FONCTION;STATUT;NBHEURES/SEM;ANCIENNETE;TYPE DE CONTRAT;SALAIRE\n");
		// ON ECRIT DANS LE FICHIER CSV LES DONNEES
		for(i=1;i<=nbEmploye;i++)
		{
			fprintf(fresBE, "%-8s;%-26s;%-26s;%-1s;%02d/%02d/%4d ;%02d/%02d/%4d ;%-10s ;%-25s ;%-14s ;%2d;%2d;%s;%8.2f\n", 
			deb->num, deb->nom, deb->prenom,deb->sexe ,deb->jourN, deb->moisN, deb->anneeN, deb->jourE, deb->moisE, 
			deb->anneeE, deb->type, deb->fonction, deb->statut,deb->heureSem,deb->anciennete,deb->typeContrat,deb->salaire);
			deb=deb->suivant;
		}
		
		// ON CONCATENE LE MOIS, L'ANNEE ET LE NOM DU FICHIERS POUR DEFINIR LE NOUVEAU NOM DU FICHIER
		sprintf(mois, "%d", moisJ);
		sprintf(annee, "%d", anneeJ);
		strcat(mois,"_");
		strcat(mois,annee);
		strcat(mois,"_");
		strcat(mois,nomFichiers2);
		strcat(mois,".csv");
		
		// ON DECLARE ET ON OUVRE LE FICHIER AU FORMATS CSV
		FILE *fresBS;
		fresBS=fopen(mois,"w");
		
		// ON ECRIT DANS LE FICHIER CSV LES DONNEES
		for(i=1;i<=nbService;i++)
		{
			fprintf(fresBS, "%5.2f ;%-26s ;%5.2f\n", debSer->tarifHoraire, debSer->nom, debSer->indexation);
			debSer=debSer->suivant;
		}
		
		// ON FERME LES FICHIERS CSV
		fclose(fresBE);
		fclose(fresBS);
	
	printf("LES FICHIERS ONT CORRECTEMENT ETE EXPORTES AU FORMATS CSV\n");
	system("PAUSE");
	}	
}

/*
	Cette fonction sert a effectuer une recherche par numéro d'identification
*/

void rechercheEmploye(Employe *deb,int nbEmploye)
{
	// DECLARATIONS DES VARIABLES
	int choix,i,n=0;
	char reponse[9];
	
	Employe *cour;

	// ON DEMANDE LE NUM A L'UTILISATEUR
	printf("Introduisez l'ID de l'employe souhaite pour la recherche :");
	scanf("%8s",&reponse);
	
	// ON CONVERTIT EN MAJUSCULE
	convertirMaju(reponse);
	
	// AFFICHAGE DES TITRES
	printf("\n\n\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|                                                                             INFORMATIONS RELATIF A L'IDENTIFIANT : %-8s                                                                                      |\n",reponse);
	printf("|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
	printf("| Num  |    ID    |            NOM             |           PRENOM           | DATE NAIS. | DATE EMBA. |      TYPE       |          SERVICE           | ANCIENNETE | HEURES/SEM |    CONTRAT    |      SALAIRE      |\n");
	printf("|------+----------+----------------------------+----------------------------+------------+------------+-----------------+----------------------------+------------+------------+---------------+-------------------|\n");
	
	cour=deb;
	// ON AFFICHE L'EMPLOYE CORRESPONDANT AU NUM
	for(i=1;i<=nbEmploye;i++)
	{
		if((strcmp(reponse, cour->num)==0))
		{
			printf("|  %02d  | %-8s | %-26s | %-26s | %02d/%02d/%4d | %02d/%02d/%4d | %-15s | %-22s |   %02d ans   |     %2dh    | %-13s | %8.2fEuro brut |\n", 
			i, cour->num, cour->nom, cour->prenom, cour->jourN, cour->moisN, cour->anneeN, cour->jourE, cour->moisE, 
			cour->anneeE, cour->type, cour->fonction, cour->anciennete,cour->heureSem,cour->typeContrat, cour->salaire);
			n++;
		}
		cour=cour->suivant;
	}
	if(n==0)
	{
		printf("\n\n                    Aucun employe ayant pour identifiant : %-8s n'a ete trouver\n",reponse);	
		printf("                    Merci de reessayer et d'indiquer un identifiant correct\n\n");	
	}
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	if(n>0)
	{
		printf("Voici les informations relatives a l'identifiant : %-8s\n\n\n\n\n", reponse);
	}
	printf("\n\n");
}

/*
	Cette fonction sert a afficher le menu principale
*/

void menuPrincipal(Employe **deb, int *nb, Service **debServ, int *nbService)
{
	// DECLARATIONS DES VARIABLES
	char quitterProg[1], choix2[1];
	
	system( clear );
	
	// AFFICHAGE DU MENU
	printf( "                  _____                            _____                   \n"); 
	printf( "                 /\\    \\                          /\\    \\                  \n");
	printf( "                /::\\    \\                        /::\\____\\                 \n");
	printf( "               /::::\\    \\                      /:::/    /                 \n");
	printf( "              /::::::\\    \\                    /:::/    /                  \n");
	printf( "             /:::/\\:::\\    \\                  /:::/    /                   \n");
	printf( "            /:::/__\\:::\\    \\                /:::/____/                    \n");
	printf( "           /::::\\   \\:::\\    \\              /::::\\    \\                    \n");
	printf( "          /::::::\\   \\:::\\    \\            /::::::\\    \\   _____           \n");
	printf( "         /:::/\\:::\\   \\:::\\____\\          /:::/\\:::\\    \\ /\\    \\          \n");
	printf( "        /:::/  \\:::\\   \\:::|    |        /:::/  \\:::\\    /::\\____\\         \n");
	printf( "        \\::/   |::::\\  /:::|____|        \\::/    \\:::\\  /:::/    /         \n");
	printf( "         \\/____|:::::\\/:::/    /          \\/____/ \\:::\\/:::/    /          \n");
	printf( "               |:::::::::/    /                    \\::::::/    /           \n");
	printf( "               |::|\\::::/    /                      \\::::/    /            \n");
	printf( "               |::| \\::/____/                       /:::/    /             \n");
	printf( "               |::|  ~|                            /:::/    /              \n");
	printf( "               |::|   |                           /:::/    /               \n");
	printf( "               \\::|   |                          /:::/    /                \n");
	printf( "                \\:|   |                          \\::/    /                 \n");
	printf( "                 \\|___|                           \\/____/                  \n");
	printf( "\n");

	while(strcmp(choix2,"0")!=0)
	{
		                  
	   	printf( "\n\n--------------------------------------------------------------------------------\n" );
	    printf( "|                               MENU PRINCIPAL                                 |\n" );
	    printf( "|------------------------------------------------------------------------------|\n" );
	    printf( "| 1: Employes                                                                  |\n" );
	    printf( "| 2: Services                                                                  |\n" );
	    printf( "| 3: Effectuer un backups                                                      |\n" );
	    printf( "| 4: Exporter en feuille de calcul (.csv)                                      |\n" );
	    printf( "| 0: Quitter le programme                                                      |\n" );
	    printf( "|------------------------------------------------------------------------------|\n" );
	    printf( "| Que voudriez-vous faire ? " );
	    // LECTURE DU CHOIX
	    scanf( "%s", &choix2 );
	    
	    system( clear );
		if(strcmp(choix2,"1")==0)
	    {
	    	// ON AFFICHE LE MENU EMPLOYE
	    	menuEmploye(&*deb, &*nb, &*debServ, &*nbService);	
		}
		else if(strcmp(choix2,"2")==0)
		{
			// ON AFFICHE LE MENU SERVICE
			menuService(&*deb, &*nb, &*debServ, &*nbService);
		}
		else if(strcmp(choix2,"3")==0)
		{
			// ON APPPEL LA FONCTION POUR EFFECTUER UNE BACKUP
			backup(*deb,*nb,*debServ,*nbService);
			// ON AFFICHE LE MENU PRINCIPAL
			menuPrincipal(&*deb,&*nb, debServ, nbService);
		}
		else if(strcmp(choix2,"4")==0)
		{
			// ON APPPEL LA FONCTION POUR EXPORTER AU FORMAT CSV
			exporterCSV(*deb,*nb,*debServ,*nbService);
			// ON AFFICHE LE MENU PRINCIPAL
			menuPrincipal(&*deb,&*nb, debServ, nbService);
		}
		else if(strcmp(choix2,"0")==0)
		{
			// ON DEMANDE SI L'UTILISATEUR SOUHAITE VRAIMENT QUITTER LE PROGRAMME ET ON LIT LE CHOIX
			printf("VOULEZ-VOUS VRAIMENT QUITTER LE PROGRAMME ? (o/n)\n");
			scanf("%s",&quitterProg);
			// SI LE CHOIX EST o ALORS ON ENREGISTRE LES DONNEES ET ON QUITTE LE PROGRAMME
			if(strcmp(quitterProg,"o")==0)
			{
				printf("VOULEZ-VOUS SAUVEGARDER VOTRE TRAVAIL ? (o/n)\n");
				scanf("%s",&quitterProg);
				if(strcmp(quitterProg,"o")==0)
				{	
					printf("SAUVEGARDE DES DONNEES\n");
					// APPEL DE LA FONCTION POUR ENREGISTRER LES DONNEES DANS LES FICHIERS DE DONNEES
					enregistrerListe(*deb, *nb, *debServ, *nbService);
					system("PAUSE");
					printf("LES DONNEES ONT ETE SAUVEGARDEES\n");
					printf("MERCI POUR VOTRE UTILISATION !\n");	
					// QUITTE LE PROGRAMME
					exit( EXIT_SUCCESS ); 
				}
				else 
				{
					printf("MERCI POUR VOTRE UTILISATION !\n");
					// QUITTE LE PROGRAMME
					exit( EXIT_SUCCESS ); 
				}
			}
			else 
			{
				// SI LE CHOIX N'EST PAS o ON RETOURNE AU MENU PRINCIPAL
				menuPrincipal(&*deb,&*nb, debServ, nbService);
			}
		}
		else
		{
			// SI L'UTILISATEUR ENCODE UNE VALEURS ERRONE, ON LUI INDIQUE ET ON RETOURN AU MENU PRINCIPAL
			printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
			system("PAUSE");
			menuPrincipal(&*deb,&*nb, debServ, nbService);
		}
	}
}

/*
	Cette fonction sert a afficher le menu employé
*/

void menuEmploye(Employe **deb, int *nb, Service **debServ, int *nbService)
{
	// DECLARATIONS DES VARIABLES
    int choix2;
    char choix[1];
    
    // AFFICHAGE DE LA LISTE DES EMPLOYES
    afficherListe(*deb, *nb);
    
    // AFFICHAGE DU MENU EMPLOYES
   	printf( "\n\n--------------------------------------------------------------------------------------\n");
    printf( "|                                   MENU EMPLOYES                                    |\n");
    printf( "|------------------------------------------------------------------------------------|\n");
    printf( "| 1: Ajouter un employe                                                              |\n");
    printf( "| 2: Supprimer employe                                                               |\n");
    printf( "| 3: Modifier un employe                                                             |\n");
    printf( "| 4: Afficher les employes d'un service                                              |\n");
    printf( "| 5: Modifier le statut d'un employe                                                 |\n");
    printf( "| 6: Afficher les statuts d'employe                                                  |\n");
    printf( "| 7: Rechercher un employe                                                           |\n");
	printf( "| 8: Anniversaire du mois                                                            |\n");
    printf( "| 0: Menu principal                                                                  |\n");
    printf( "|-------------------------------------------------------------------------------------\n");
    printf( "|Que voudriez-vous faire ? " );
    // ON LIT LE CHOIX
    scanf( "%s", &choix );

	system(clear);

    if(strcmp(choix,"1")==0)
    {
    	// ON APPEL LA FONCTION POUR AJOUTER UN EMPLOYE
		ajouterEmploye(&*deb, &*nb, *debServ, *nbService);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);	
	}
	else if(strcmp(choix,"2")==0)
	{
		// ON APPEL LA FONCTION POUR SUPPRIMER UN EMPLOYE
		supprimerEmploye(&*deb, &*nb);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, &*debServ, &*nbService);
	}
	else if(strcmp(choix,"3")==0)
	{
		// ON APPEL LA FONCTION POUR MODIFIER UN EMPLOYE
		modifierEmploye(*deb, *nb, *debServ, *nbService);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"4")==0)
	{
		// ON APPEL LA FONCTION POUR AFFICHER UN EMPLOYE SELON UN SERVICE
		afficherEmpServ(*deb, *nb, *debServ, *nbService);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"5")==0)
	{
		// ON APPEL LA FONCTION POUR MODIFIER LE STATUT D'UN EMPLOYE
		modifierStatutEmp(deb,nb);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"6")==0)
	{
		// ON APPEL LA FONCTION POUR AFFICHER DES EMPLOYES EN FONCTION DU STATUT
		afficherEmpStatut(*deb, *nb);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"7")==0)
	{
		// ON APPEL LA FONCTION POUR AFFICHER LES ANNIVERSAIRE DU MOIS EN COURS
		rechercheEmploye(*deb,*nb);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"8")==0)
	{
		// ON APPEL LA FONCTION POUR AFFICHER LES ANNIVERSAIRE DU MOIS EN COURS
		anniversaireMois(*deb, *nb);
		// ON AFFICHE LE MENU EMPLOYE
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
	else if(strcmp(choix,"0")==0)
	{
		// ON AFFICHE LE MENU PRINCIPAL
		menuPrincipal(&*deb,&*nb, &*debServ, &*nbService);
	}
	else
	{
		// SI L'UTILISATEUR ENCODE UNE VALEURS ERRONE, ON LUI INDIQUE ET ON RETOURN AU MENU EMPLOYE
		system(clear);
		printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
		system("PAUSE");
		system(clear);
		menuEmploye(&*deb, &*nb, debServ, nbService);
	}
}

/*
	Cette fonction sert a afficher le menu service
*/

void menuService(Employe **deb, int *nb, Service **debServ, int *nbService)
{
	// DECLARATIONS DES VARIABLES
    int choix2;
    char choix[1];
    
    // AFFICHAGE DE LA LISTE DES SERVICES
    afficherListeService(*debServ, *nbService);
	
    // AFFICHAGE DU MENU DES SERVICES
	printf( "\n\n--------------------------------------------------------------------------------------\n");
    printf( "|                                    MENU SERVICE                                    |\n");
    printf( "|------------------------------------------------------------------------------------|\n");
    printf( "| 1: Ajouter un service                                                              |\n");
    printf( "| 2: Supprimer un service                                                            |\n");
    printf( "| 3: Modifier un service                                                             |\n");
    printf( "| 0: Menu principal                                                                  |\n");
    printf( "|-------------------------------------------------------------------------------------\n");
    printf( "| Que voudriez-vous faire ? " );
    // ON LIT LE CHOIX
    scanf( "%s", &choix );

	system(clear);

    if(strcmp(choix,"1")==0)
    {
    	// ON APPEL LA FONCTION POUR AJOUTER UN SERVICE
		ajouterService(&*debServ, &*nbService);
		// ON AFFICHE LE MENU SERVICE
		menuService(&*deb, &*nb, &*debServ, &*nbService);	
	}
	else if(strcmp(choix,"2")==0)
    {
    	// ON APPEL LA FONCTION POUR SUPPRIMER UN SERVICE
		supprimerService(&*debServ, &*nbService);
		// ON AFFICHE LE MENU SERVICE
		menuService(&*deb, &*nb, &*debServ, &*nbService);	
	}
	else if(strcmp(choix,"3")==0)
    {
    	// ON APPEL LA FONCTION POUR MODIFIER UN EMPLOYE
		modifierService(*debServ,*nbService);
		// ON AFFICHE LE MENU SERVICE
		menuService(&*deb, &*nb, &*debServ, &*nbService);	
	}
	else if(strcmp(choix,"0")==0)
	{
		// ON AFFICHE LE MENU PRINCIPAL
		menuPrincipal(&*deb,&*nb, &*debServ, &*nbService);
	}
	else
	{
		// SI L'UTILISATEUR ENCODE UNE VALEURS ERRONE, ON LUI INDIQUE ET ON RETOURN AU MENU SERVICE
		system(clear);
		printf("ERREUR, VEUILLEZ ENTREZ UNE VALEUR VALIDE\n");
		system("PAUSE");
		system(clear);
		menuService(&*deb, &*nb, &*debServ, &*nbService);
	}
}

/*
	Le main sert a la lecture des fichiers dat.
*/

main()
{
	// AFFICHAGE DE LA CONSOLE D'EXECUTION EN PLEIN ECRAN
	HWND hwnd=GetForegroundWindow();
	ShowWindow(hwnd,SW_MAXIMIZE); 
	
	// ON OUVRE LES FICHIERS DE DONNEES
	FILE *fdat,*fdat2;
	fdat=fopen("ManzanoGodemont.dat","r");
	fdat2=fopen("ManzanoGodemont02.dat", "r");

	// DECLARATIONS DES VARIABLES
	int i,j,nbEmploye=0,numero,place, nbService=0;
	
	Service *debSer, *courantSer, *suivantSer;
    debSer=malloc(sizeof(Service));
    courantSer=debSer;
    
    // LECTURE DES DONNEES DU FICHIERS SERVICE
    fscanf(fdat2, "%5f ", &courantSer->tarifHoraire);
	while(!feof(fdat2))
    {
    	suivantSer=malloc(sizeof(Service));
    	fgets (courantSer->nom , 27 , fdat2);
    	fscanf(fdat2,"%5f ", &courantSer->indexation);
    	(*courantSer).suivant=suivantSer;
    	// ON COMPTE LE NOMBRE DE SERVICE
    	nbService++;
    	courantSer=suivantSer;
    	fscanf(fdat2, "%5f ", &courantSer->tarifHoraire);
	}
	courantSer=debSer;
	for(i=1;i<nbService;i++)
		courantSer=courantSer->suivant;
	(*courantSer).suivant=NULL;
	
	Employe *deb, *courant, *suivant;
    deb=malloc(sizeof(Employe));
    courant=deb;
    
    // LECTURE DES DONNEES DU FICHIERS SERVICE
    fscanf(fdat, "%8s", &courant->num);
	while(!feof(fdat))
    {
    	suivant=malloc(sizeof(Employe));
    	fgets (courant->nom , 27 , fdat);
	    fgets (courant->prenom , 27 , fdat);
	    fscanf(fdat, "%2d/%2d/%4d %2d/%2d/%4d %10s ", 
		&courant->jourN, &courant->moisN, &courant->anneeN, &courant->jourE, &courant->moisE, 
		&courant->anneeE, &courant->type);
	    fgets (courant->fonction , 27 , fdat);
		fscanf(fdat, "%13s %1s %2d", &courant->statut,&courant->sexe,&courant->heureSem);
		courant->anciennete=calculerAnciente(courant->jourE, courant->moisE, courant->anneeE);
		courantSer=debSer;
		for(i=1;i<=nbService;i++)
		{
			if(strcmp(courant->fonction, courantSer->nom)==0)
			{
				break;
			}
			courantSer=courantSer->suivant;
		}
		typeContract(courant);
		courant->salaire=calculSalaire(courantSer->tarifHoraire,courantSer->indexation, &*courant);
    	(*courant).suivant=suivant;
    	// ON COMPTE LE NOMBRE D'EMPLOYE
    	nbEmploye++;
    	courant=suivant;
    	fscanf(fdat, "%8s", &courant->num);
	}
	courant=deb;
	for(i=1;i<nbEmploye;i++)
		courant=courant->suivant;
	(*courant).suivant=NULL;
	
	// ON FERME LES FICHIERS DE DONNEES
	fclose(fdat);
	fclose(fdat2);
	
	// ON AFFICHE LE MENU PRINCIPAL
	menuPrincipal(&deb, &nbEmploye, &debSer, &nbService);
}
