#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define TAILLE_SECTEUR 512


// TP réalisé par REMACHE Amine , 1CS , G6 , section B

// Prototypes


void menu()
{
    printf("\n\n\n");
    printf("1) Lire un secteur.\n");
    printf("2) Afficher un secteur.\n");
    printf("3) Recherche d'une chaine en Hexadecimale.\n");
    printf("4) Afficher les disques.\n");
    printf("5) Changer le choix du disque.\n");
    printf("0) Quitter.\n");
    printf("\n\n\n");

}


char *Lire_secteur (  int num_secteur , int TailleSecteur, char *chemin_disque_choisit ) {
    
    char *buffer=malloc(sizeof(char)*TailleSecteur);

    FILE *disque = fopen(chemin_disque_choisit,"r");
    
    if ( disque != NULL ) 
    {
        
        fseek(disque, num_secteur*TailleSecteur, SEEK_SET);
        
        for(int i=0; i<TailleSecteur; i++) 
        {
            buffer[i]=getc(disque);
        }
        
        
        fclose(disque);
        
    }
    
    return buffer;
}


void Afficher_secteur ( int num_secteur, int TailleSecteur, char *chemin_disque_choisit , char *buffer ) {
    
    buffer = Lire_secteur(num_secteur,TailleSecteur ,chemin_disque_choisit);
    
    int i=0,j=0;
    
    
    
    for(i=0; i<TailleSecteur; i++)
    {

        if(i%16==0)
        {
            printf("\n%.4d   ",i);
            j=j+16;
        }
                
        printf("%3.2hhX ",buffer[i]);
        
    }
    printf("\n");
    
}



void Choix_Disque ( char *chemin_disque_choisit , char **list_des_disques ) {
   
            
    
initial:    printf("\nVeuillez introduire le nom du disque avec lequel vous voudriez travailler :\n");
            printf("# introduire une chaîne composé du type du disque : 'hd' ou 'sd'\n");
            printf("# concaténé à une lettre de 'a' à 'z' , mais d'un disque qui est affiché dans la liste précédente\n");
            char temp[8]; strcpy(temp,"/dev/");
            int continuer=0,i=0;
            scanf("%s",&temp[5]);
            
    
    while (i<52 && (continuer==0)) {
    
        if ( strcmp(list_des_disques[i],temp) == 0 ) {
            strcpy(chemin_disque_choisit,temp);
            printf("\nVous avez choisi le disque : '%s'\n\n",chemin_disque_choisit);
            continuer=1;
        }
        
        i++;
        
    }
    
    if ( i >= 52 ) 
    {
        
        printf("\nLe disque que vous avez choisit n'existe pas !\n");
        goto initial;
    }

}



void Liste_Disques (char **list_des_disques) {

    char num_disque_physique = 'a';  
    char chemin_disque[8];
    FILE *disque;
    int i=0;

    printf("\nVoici la liste des disques connectés à votre machine :\n");
    
    // Boucle pour rechercher les disques de type SCSI
    while (i<26) {
    
        strcpy( chemin_disque , "/dev/sd");
        chemin_disque[7] = num_disque_physique+i;
        disque=fopen(chemin_disque,"r");
        // Si on peut ouvrir le fichier , c'est qu'il existe un disque lié à la machine portant le nom sdX , où X est une lettre de 'a' à 'z'
        if (disque != NULL) {
        
            // Remarque : il pourrait y avoir aucun résultat si vous n'êtes pas en mode admin ( sudo ) pour des raisons de droits de lecture
            printf("\nLe disque N°%d se trouve à : %s\n",i+1,chemin_disque);
        
            strcpy(list_des_disques[i],chemin_disque);
        
        }
        
        else  strcpy(list_des_disques[i],"/dev/sd0");
        
        
        i++;
        
    }
    
    // Boucle pour rechercher les disques de type ATA
    while (i<52) {
    
        strcpy( chemin_disque , "/dev/hd");
        chemin_disque[7] = num_disque_physique+i;
        disque=fopen(chemin_disque,"r");
        // Si on peut ouvrir le fichier , c'est qu'il existe un disque lié à la machine portant le nom sdX , où X est une lettre de 'a' à 'z'
        if (disque != NULL) {
        
            // Remarque : il pourrait y avoir aucun résultat si vous n'êtes pas en mode admin ( sudo ) pour des raisons de droits de lecture
            printf("\nLe disque N°%d se trouve à : %s\n",i+1,chemin_disque);

            strcpy(list_des_disques[i],chemin_disque);
        
        }
        
        else  strcpy(list_des_disques[i],"/dev/hd0");
        
        
        
        i++;
    }
        
        
}



void recherche (int num_secteur , int TailleSecteur, char *chemin_disque_choisit , char *buffer) {

    int pointeur_chaine=0;
    int pointeur_text=0;
    int continuer = 1;
    int temporaire;
    int stop=0;
    //int *tabl = malloc(sizeof(int)*TailleSecteur);
    char chaine[]="un";
    char text[]="je suis un"; 
    
    char *occurence=malloc(sizeof(char)*TailleSecteur);
    char *lire_sect;
    
    
    lire_sect = Lire_secteur(num_secteur , TailleSecteur, chemin_disque_choisit );

    while ( continuer && pointeur_text < 512 ) 
    {
        
        pointeur_chaine=0;
        
        if (chaine[pointeur_chaine] == lire_sect[pointeur_text] ) 
        {
            
            temporaire=pointeur_text;
            temporaire++; // on va use this to parcourir au lieu de pointeur_text
            pointeur_chaine++;
            
            
            stop =0;
            
            while ( temporaire<512 && !stop ) 
            {
                printf("\nDans le while pointeur_chaine=%d\n",pointeur_chaine);
                if ( chaine[pointeur_chaine] == lire_sect[temporaire] ) 
                {
                    temporaire++; pointeur_chaine++;
                }
                if ( pointeur_chaine == strlen(chaine) )
                {
                    stop = 1;
                }
                else 
                {
                    pointeur_text++;
                    pointeur_chaine=0;
                    stop = 1;
                }
                
            }
            
            printf("\npointer_chaine=%d\nstrlen(chaine)=%zd",pointeur_chaine,strlen(chaine));
            
            if ( temporaire <= 512 && pointeur_chaine == strlen(chaine) ) 
            {
                continuer = 0;
                printf("\nchaine retrouvée !\n");
            }
            else printf("\nchaine non trouvee !\n");
            
            
        }
        else pointeur_text++;
        
        
    }
    
    //return tabl;
}




int main () {
    
    
    char **list_des_disques=malloc(52*sizeof(char*));
    for (int i=0;i<52;i++) list_des_disques[i]=malloc(8*sizeof(char));
    
    char *chemin_disque_choisit=malloc(8*sizeof(char));
    
    char *buff = malloc ( sizeof(char)*TAILLE_SECTEUR);
    
    int choix_menu=0,quitter=0;
    
    int num_secteur,secteur_debut,secteur_fin;
    
    char chaineRecherche[1024];
    

    Liste_Disques (list_des_disques);
    Choix_Disque (chemin_disque_choisit,list_des_disques);
    
    
    while (!quitter) {
        
        
        menu();
        printf("Veuillez introduire votre choix :\n");
        scanf("%d",&choix_menu);    
        
        switch ( choix_menu )
        {
        
        case 1:
            printf("\nVeuillez donner le numero de secteur à lire :\n");
            scanf("%d",&num_secteur);
            buff=Lire_secteur(num_secteur,TAILLE_SECTEUR,chemin_disque_choisit);
            printf("\nSecteur N°%d lu avec succès\n",num_secteur);
            
        break;
        
        case 2:
            printf("\nVeuillez donner le numero de secteur à afficher :\n");
            scanf("%d",&num_secteur);
            Afficher_secteur(num_secteur,TAILLE_SECTEUR,chemin_disque_choisit,buff);
            
        break;
        
        case 3:
            
            printf("\nVeuillez donner le numero de secteur dans lequel rechercher :\n");
            scanf("%d",&num_secteur);
            recherche(num_secteur,TAILLE_SECTEUR,chemin_disque_choisit,buff);
            
        break;
        
        case 4:
            Liste_Disques (list_des_disques);
            
            
        break;
        
        case 5:
            Choix_Disque (chemin_disque_choisit,list_des_disques);
            printf("\nDisque change avec succes !\n");
            
        break;
        
        case 0:
            printf("\n\nMerci d'avoir utilisé mon programme !\n\n");
            quitter=1;
        break;
        
        }
        
    
    }

    return 0;
}
