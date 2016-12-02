#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include "iniedit.c"*/
#include "iniedit.h"

void vypisZaznamy(int showComments, int showKeys, int showValues, char *key){
  int i,j,k,count;
  _ZAZNAMY* records;
  
  
  records=getAllRecords();
  count=getRecordsCount(NULL,NULL);
  
  
  for(i=0;i<count;i++){
      /*ak nehladam konkretny kluc alebo ak konkretny kluc = hladanemu*/
      if(key==NULL || strcmp(key, records[i].kluc)==0){
	/*ak chcem vypisat aj komentare*/
	if(showComments==1){
	  for(k=0;k<records[i].commentsCount;k++){
	    printf("%s\n",records[i].comments[k].komentar);
	  }
	}
	/*ak chcem vypisat kluce*/
	if(showKeys==1){
	  printf("%s",records[i].kluc);
	  if(showValues==1)printf("=");
	}
	/*ak chcem vypisat hodnoty*/
	if(showValues==1){
	  printf("%s",records[i].hodnota);
	}
	
	printf("\n");
      }
    
  }
}

void printMenu(){
  puts("               1: nacitat subor                               2: ulozit subor              ");
  puts("               3: vypisat sekcie                              4: vypisat zaznamy v sekcii  ");
  puts("               5: vypisat vsetky zaznamy                      6: najst hodnotu zaznamu     ");
  puts("               7: pridat zaznam                               8: prepisat zaznam           ");
  puts("               9: odstranit zaznam                            10: ukoncit program          ");
  printf("	zadaj svoju volbu: ");
}

int main(int argc, char *argv[]){
  
  int volba,i,j,sections;
  char subor[MAX_LINE];
  char kluc[RECORD_KEY];
  char sekc[SEC_NAME];
  char hodnota[RECORD_VALUE];
  
  _SEKCIE* sekcia=NULL;
  
  _ZAZNAMY *r=NULL;
  
  
  int pocet=0;
  char *s;
  
  if(argv[1]!=NULL){
    if(nacitajSubor(argv[1])==EXIT_SUCCESS){
      printf("subor '%s' nacitany\n\n",argv[1]);
    }else{
      printf("subor '%s' sa nepodarilo nacitat\n\n",argv[1]);
    }
  }
  
  
  while(volba!=10){
    
    _KOMENTARE *com=NULL;
    _KOMENTARE *tmpComments=NULL;
    int commCnt=0,i=0;
    
    printMenu();
    scanf("%d",&volba);
    switch(volba){
      case 1: printf("Zadaj nazov suboru: ");
	      scanf("%s", subor);
	      if(nacitajSubor(subor)==EXIT_SUCCESS){
		printf("subor '%s' nacitany\n\n",subor);
		sekcia=getAllSections();
		sections=getSectionCount();
	      }else{
		printf("subor '%s' sa nepodarilo nacitat\n\n",subor);
	      }
	      break;
      case 2: printf("Zadaj nazov suboru: ");
	      scanf("%s", subor);
	      if(ulozSubor(subor)==EXIT_SUCCESS){
		printf("subor '%s' ulozeny\n\n",subor);
	      }
	     
	      break;
      case 3: for(i=0;i<sections;i++){
		for(j=0;j<sekcia[i].commentsCount;j++){
		  printf("#%s\n",sekcia[i].comments[j].komentar);
		}
		printf("%s\n\n",sekcia[i].meno);
	      }
	      break;
      case 4: printf("Zadaj nazov sekcie: ");
	      scanf("%s", sekc);
	      r=getSectionRecords(sekc);
	      pocet=getRecordsCount(sekc,NULL);
	      for(i=0;i<pocet;i++){
		for(j=0;j<r[i].commentsCount;j++){
		  printf("#%s\n",r[i].comments[j].komentar);
		}
		printf("%s=%s\n\n",r[i].kluc, r[i].hodnota);
	      }
	      break;
      case 5: r=getAllRecords();
	      pocet=getRecordsCount(NULL,NULL);
	      for(i=0;i<pocet;i++){
		printf("%s=%s\n",r[i].kluc, r[i].hodnota);
	      }
	      break;
      case 6: printf("Zadaj hladany kluc: ");
	      scanf("%s", kluc);
	      pocet=getRecordsCount(NULL,kluc);
	      r=getRecords(NULL,kluc);
	      for(i=0;i<pocet;i++){
		for(j=0;j<r[i].commentsCount;j++){
		  printf("#%s\n",r[i].comments[j].komentar);
		}
		printf("%s=%s\n\n",r[i].kluc,r[i].hodnota);
	      }
	      break;
      case 7: printf("zadaj sekciu do ktorej sa prida zaznam: ");
	      scanf("%s", sekc);
	      if(getRecordsCount(sekc,NULL)>0){
		printf("sekcia %s existuje\n",sekc);
	      }else{
		printf("vytvori sa nova sekcia\n",sekc);
	      }
	      printf("zadaj kluc: ");
	      scanf("%s", kluc);
	      if(getRecordsCount(sekc,kluc)>0){
		printf("Pridanie neuspesne, zaznam uz existuje\n");
	      }else{
		printf("zadaj hodnotu: ");
		scanf("%s", hodnota);
		if(pridajZaznamPrepis(sekc, kluc, hodnota)==0){
		  sekcia=getAllSections();
		  sections=getSectionCount();
		  printf("Zaznam uspesne pridany\n");
		}else{
		  printf("Pridanie neuspesne\n");
		}
	      }
	      break;
      case 8: printf("zadaj sekciu do ktorej sa prida/prepise zaznam: ");
	      scanf("%s", sekc);
	      printf("zadaj kluc: ");
	      scanf("%s", kluc);
	      pocet=getRecordsCount(sekc,kluc);
	      if(pocet>0){
		printf("V sekcii %s uz existuje zaznam %s=%s\n",sekc,kluc,getRecords(sekc,kluc)[0].hodnota);
	      }else{
		printf("V sekcii %s este nie je zaznam %s\n",sekc,kluc);
	      }
	      printf("zadaj novu hodnotu: ");
	      scanf("%s", hodnota);
	      if(pridajZaznamPrepis(sekc, kluc, hodnota)==0){
		if(pocet>0){
		  printf("Zaznam uspesne prepisany\n");
		}else{
		  printf("Zaznam uspesne pridany\n");
		}
		sekcia=getAllSections();
		sections=getSectionCount();
	      }else{
		if(pocet>0){
		  printf("Prepisanie neuspesne\n");
		}else{
		  printf("Pridanie neuspesne\n");
		}
	      }
	      break;
      case 9: printf("zadaj sekciu z ktorej sa zmaze zaznam: ");
	      scanf("%s", sekc);
	      printf("zadaj kluc: ");
	      scanf("%s", kluc);
	      if(getRecordsCount(sekc,kluc)==0){
		printf("V sekcii %s nie je zaznam %s\n",sekc,kluc);
	      }else{
		if(vymazZaznam(sekc, kluc)==1){
		  printf("Zaznam uspesne zmazany\n");
		}else{
		  printf("Mazanie neuspesne\n");
		}
		sekcia=getAllSections();
		sections=getSectionCount();
	      }
	      break;
      default: break;
      volba=0;
    }
  }

  freememory();
  
  return EXIT_SUCCESS;
}
