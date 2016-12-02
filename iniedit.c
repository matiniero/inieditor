#include "iniedit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


_SEKCIE *sekcia=NULL;
int sections=0;

int ulozSubor(char *filename){
  FILE *iniFile=NULL;
  char buffer[MAX_LINE];
  
  int records,comments;
  int i,j,k;
  
  iniFile=fopen(filename, "w");
  if(iniFile==NULL)return EXIT_FAILURE;
  
  for(i=0;i<sections;i++){
    for(j=0;j<sekcia[i].commentsCount;j++){
     fprintf(iniFile,"#%s\n",sekcia[i].comments[j].komentar); 
    }
    fprintf(iniFile,"[%s]\n",sekcia[i].meno);
    for(j=0;j<sekcia[i].recordsCount;j++){
      for(k=0;k<sekcia[i].records[j].commentsCount;k++){
	fprintf(iniFile,"#%s\n",sekcia[i].records[j].comments[k].komentar); 
      }
      fprintf(iniFile,"%s=%s\n",sekcia[i].records[j].kluc,sekcia[i].records[j].hodnota);
    }
    fprintf(iniFile,"\n");
  }
  fclose(iniFile);
  return EXIT_SUCCESS;
}

int vymazZaznam(char *sekc, char *kluc){
  int i,j,k;
  _ZAZNAMY *tmpr;
  for(i=0;i<sections;i++){
    /*najdem sekciu*/
    if(strcmp(sekc, sekcia[i].meno)==0){
      for(j=0;j<sekcia[i].recordsCount;j++){
	/*najdem zaznam*/
	if(strcmp(sekcia[i].records[j].kluc,kluc)==0){
	  free(sekcia[i].records[j].comments);
	  for(k=j+1;k<sekcia[i].recordsCount;k++){
	    sekcia[i].records[k-1]=sekcia[i].records[k];
	  }
	  tmpr=(_ZAZNAMY*)realloc(sekcia[i].records, sizeof(_ZAZNAMY)*(sekcia[i].recordsCount-1));
	  sekcia[i].records=tmpr;
	  sekcia[i].recordsCount--;
	  return 1;
	}
      }
      /*zaznam nenajdeny*/
      return 0;
    }
  }
  /*sekcia nenajdena*/
  return 0;
}

char* getStringValue(char *sekciameno, char *kluc){
  _ZAZNAMY* tmp=getRecords(sekciameno, kluc);
  return tmp[0].hodnota;
}

int getIntValue(char *sekciameno, char *kluc){
  _ZAZNAMY* tmp=getRecords(sekciameno, kluc);
  return atoi(tmp[0].hodnota);  
}

_ZAZNAMY* getRecords(char *sectionName,char *recordKey){
  int i,j;
  int cnt=0,ptr=0;
  _ZAZNAMY *res;
  
  cnt=getRecordsCount(sectionName,recordKey);
  
  res=malloc(sizeof(_ZAZNAMY)*cnt);
  
  /*ak hladam vo vsetkych sekciach*/
  if(sectionName==NULL){
    for(i=0;i<sections;i++){
      /*ak hladam konkretny zaznnam*/
      if(recordKey!=NULL){
	/*prebehnem a porovnam zaznamy v sekcii*/
	for(j=0;j<sekcia[i].recordsCount;j++){
	  if(strcmp(sekcia[i].records[j].kluc,recordKey)==0){
	    res[ptr]=sekcia[i].records[j];
	    ptr++;
	  }
	}
      }else{/*ak hladam vsetky zaznamy*/
	for(j=0;j<sekcia[i].recordsCount;j++){
	  res[ptr]=sekcia[i].records[j];
	  ptr++;
	}
      }
    }
    return res;
  }else{/*ak hladam v konkretnej sekcii*/
    for(i=0;i<sections;i++){
      if(strcmp(sectionName, sekcia[i].meno)==0){
	if(recordKey!=NULL){
	  for(j=0;j<sekcia[i].recordsCount;j++){
	    if(strcmp(sekcia[i].records[j].kluc,recordKey)==0){
	      return &sekcia[i].records[j];
	    }
	  }
	}else{
	  return sekcia[i].records;
	}
      }
    }
    return NULL;
  }
}

_ZAZNAMY* getAllRecords(){
  return getRecords(NULL, NULL);
}

_ZAZNAMY* getSectionRecords(char *sectionName){
  return getRecords(sectionName, NULL);
}

int getRecordsCount(char *sectionName, char *recordKey){
  int i,j;
  int cnt=0;
  
  /*ak nehladam v konkretnej sekcii*/
  if(sectionName==NULL){
    for(i=0;i<sections;i++){
      if(recordKey!=NULL){
	for(j=0;j<sekcia[i].recordsCount;j++){
	  if(strcmp(sekcia[i].records[j].kluc,recordKey)==0){
	    cnt++;
	  }
	}
      }else{
	cnt+=sekcia[i].recordsCount;
      }
    }
    return cnt;
  }else{/*hladam v konkretnej sekcii*/
    /*prejdem sekcie*/
    for(i=0;i<sections;i++){
      /*ak najdem hladanu sekciu*/
      if(strcmp(sectionName, sekcia[i].meno)==0){
	/*nehladam konkretny kluc*/
	if(recordKey==NULL){
	  /*vraciam poc. klucov v sekcii*/
	  return sekcia[i].recordsCount;
	}else{/*hladam konkretny kluc*/
	  for(j=0;j<sekcia[i].recordsCount;j++){
	    /*ak najdem hladany zaznam vratim 1*/
	    if(strcmp(sekcia[i].records[j].kluc,recordKey)==0){
	      return 1;
	    }
	  }
	  /*v hladanej sekcii som nenasiel hladany kluc, vraciam 0*/
	  return 0;
	}
      }
    }
    /*nenasiel som ani sekciu ani zaznam*/
    return 0;
  }
}

int pridajSekciu(char *nazov, int pocetz, _ZAZNAMY *zaz, int pocetk, _KOMENTARE *kom){
  int i,j,c;
  _SEKCIE *tmpsekcie=NULL;
  for(i=0;i<sections;i++){
    c=strcmp(sekcia[i].meno, nazov);
    if(c==0){
      puts("sekcia uz existuje, ziadne zmeny neboli vykonane");
      return 1;
    }
  }
  sections++;
  tmpsekcie=(_SEKCIE*)realloc(sekcia,sizeof(_SEKCIE)*sections);
  
  memcpy(tmpsekcie[sections-1].meno, nazov, SEC_NAME);
  
  tmpsekcie[sections-1].records=zaz;
  tmpsekcie[sections-1].recordsCount=pocetz;
  
  tmpsekcie[sections-1].comments=kom;
  tmpsekcie[sections-1].commentsCount=pocetk;
  
  sekcia=tmpsekcie;
}

int pridajZaznamPrepis(char *sekciameno, char *kluc, char *hodnota){
  return pridajZaznam(sekciameno, kluc, hodnota, 1);
}

int pridajZaznamNeprepis(char *sekciameno, char *kluc, char *hodnota){
  return pridajZaznam(sekciameno, kluc, hodnota, 0);
}

int pridajZaznam(char *sekciameno, char *kluc, char *hodnota, int rewrite){
  int i,j,c,r;
  _ZAZNAMY *tmpzaznamy=NULL;
  for(i=0;i<sections;i++){
    c=strcmp((char*)sekcia[i].meno, sekciameno);
    /*nasiel som sekciu*/
    if(c==0){
      
      /*najdem zaznam s rovnakym klucom*/
      for(j=0;j<sekcia[i].recordsCount;j++){
	r=strcmp(sekcia[i].records[j].kluc, kluc);
	
	/*nasiel som kluc, upravim hodnotu*/
	if(r==0){
	  if(rewrite==1){
	    memcpy(sekcia[i].records[j].hodnota, hodnota, RECORD_VALUE);
	    return 0;
	  }else{
	    return 1;
	  }
	}
	
      }
      
      /*zaznam som nenasiel, musim pridat novy*/
      tmpzaznamy=(_ZAZNAMY*)realloc(sekcia[i].records, sizeof(_ZAZNAMY)*(sekcia[i].recordsCount+1));
      if(tmpzaznamy!=NULL){
	memcpy(tmpzaznamy[sekcia[i].recordsCount].kluc, kluc, RECORD_KEY);
	memcpy(tmpzaznamy[sekcia[i].recordsCount].hodnota, hodnota, RECORD_VALUE);
	
	sekcia[i].records=tmpzaznamy;
	sekcia[i].recordsCount++;
	
	return 0;
      }else{
	errorMsg("Alokacia zaznamov", 1);
      }
    }
  }
  /*nenasiel som sekciu*/
  
  /*vytvorim strukturu pre 1 zaznam*/
  tmpzaznamy=(_ZAZNAMY*)malloc(sizeof(_ZAZNAMY));
  if(tmpzaznamy==NULL){
    errorMsg("Alokacia zaznamov", 1);
    return 1;
  }
  
  tmpzaznamy[0].comments=0;
  tmpzaznamy[0].commentsCount=0;
  /*skopirujem kluc a hodnotu*/
  memcpy(tmpzaznamy[0].kluc, kluc, RECORD_KEY);
  memcpy(tmpzaznamy[0].hodnota, hodnota, RECORD_VALUE);
  
  /*pridam sekciu aj so zaznamom*/
  pridajSekciu(sekciameno, 1, tmpzaznamy, 0, NULL);
  return 0;
}

void freememory(){
  int i,j;
  for(i=0;i<sections;i++){
    for(j=0;j<sekcia[i].recordsCount;j++){
      free(sekcia[i].records[j].comments);
    }
    free(sekcia[i].records);
    free(sekcia[i].comments);
  }
  free(sekcia);
  sections=0;
  sekcia=NULL;
}

/*ak qvalue bude 1 ukonci inak len vypise chybu*/
void errorMsg(const char *text, int qValue){
  printf("ERROR: %s \n",text);
  if(qValue)exit(EXIT_FAILURE);
}

int nacitajSubor(char *filename){
  FILE *iniFile;
  char buffer[MAX_LINE];
  
  /*data ulozene v poli*/
  _KOMENTARE *komentar=NULL;
  _ZAZNAMY *zaznam=NULL;
  
  int records=0, comments=0;
  int i,j,k,l,m;
  
  /*vymazem predchadzajuce udaje*/
  freememory();
  
  iniFile=fopen(filename, "r");
  if(iniFile==NULL){
    errorMsg("Vstupny subor!",0);
    return EXIT_FAILURE;
  }
  
  while(fgets(buffer, MAX_LINE, iniFile)!=NULL){
    
    switch(buffer[0]){
      _KOMENTARE *tmpcom=NULL;
      _ZAZNAMY *tmpz=NULL;
      _SEKCIE *tmps=NULL;
      case '[': if(zaznam!=NULL && sekcia!=NULL){
		  sekcia[sections-1].records=zaznam;
		  sekcia[sections-1].recordsCount=records;
		  zaznam=NULL;
		  records=0;
		}
		sections++; 
		
		tmps=NULL;
		tmps=(_SEKCIE*)realloc(sekcia, sizeof(_SEKCIE)*sections);
		
		if(tmps!=NULL){
		  if(komentar!=NULL && comments>0){
		    tmps[sections-1].comments=komentar;
		    tmps[sections-1].commentsCount=comments;
		    komentar=NULL;
		    comments=0;
		  }else{
		    tmps[sections-1].comments=NULL;
		    tmps[sections-1].commentsCount=0;
		  }
		  
		  for(j=0;j<SEC_NAME;j++){
		    tmps[sections-1].meno[j]=0;
		  }
		  
		  j=1;
		  while(buffer[j]!=EOF && buffer[j]>=32 && buffer[j]!=']'){
		    tmps[sections-1].meno[j-1]=buffer[j];
		    j++;
		  }
		  		    
		  tmps[sections-1].records=NULL;
		  tmps[sections-1].recordsCount=0;
		  
		  sekcia=tmps;
		}else{
		  errorMsg("Alokovanie sekcii",1);
		}
		break;
      case ';': 
      case '#': comments++;
		tmpcom=(_KOMENTARE*)realloc(komentar, sizeof(_KOMENTARE)*comments);
		if(tmpcom!=NULL){
		  
		  for(j=0;j<MAX_LINE;j++){
		    tmpcom[comments-1].komentar[j]=0;
		  }
		  
		  j=1;
		  while(buffer[j]!=EOF && buffer[j]>=32){
		    tmpcom[comments-1].komentar[j-1]=buffer[j];
		    j++;
		  }
		  
		  komentar=tmpcom;
		}else{
		  errorMsg("Alokovanie komentarov",1);
		}
		break;
      default : 
	  if(buffer[0]>32 && buffer[0]!='='){
	    tmpz=NULL;
	    
	    tmpz=(_ZAZNAMY*)realloc(sekcia[sections-1].records, sizeof(_ZAZNAMY)*(sekcia[sections-1].recordsCount+1));
	    
	    for(j=0;j<RECORD_KEY;j++){
		tmpz[sekcia[sections-1].recordsCount].kluc[j]=0;
	    }
	    for(j=0;j<RECORD_VALUE;j++){
		tmpz[sekcia[sections-1].recordsCount].hodnota[j]=0;
	    }
	    
	    if(tmpz!=NULL){
	      k=0;
	      j=0;
	      while(buffer[j]!='='){
		tmpz[sekcia[sections-1].recordsCount].kluc[j]=buffer[j];
		j++;
		if(buffer[j]<=32)break;
	      }
	      /*ak je zly znak pred = skoncim*/
	      if(buffer[j-1]<32)break;
	      j++;
	      k=j;
	      /*ak je zly znak za = skoncim*/
	      if(buffer[k]<32)break;
	      
	      
	      /*v hodnote mozu byt aj prazdne znaky*/
	      while(buffer[k]!=EOF && buffer[k]>=32){
		tmpz[sekcia[sections-1].recordsCount].hodnota[k-j]=buffer[k];
		k++;
	      }
	      
	      
	      /*ak som predtym cital nejake komentare priradim ich k zaznamu*/
	      if(komentar!=NULL){
		tmpz[sekcia[sections-1].recordsCount].comments=komentar;
		tmpz[sekcia[sections-1].recordsCount].commentsCount=comments;
		komentar=NULL;
		comments=0;
	      }else{
		tmpz[sekcia[sections-1].recordsCount].comments=NULL;
		tmpz[sekcia[sections-1].recordsCount].commentsCount=0;
	      }
	      
	      records++;
	      
	      sekcia[sections-1].records=tmpz;
	      sekcia[sections-1].recordsCount++;
	      
	    }else{
	      errorMsg("Alokovanie zaznamov!",1);
	    }
	    
	  }
    }
    
  }
  return EXIT_SUCCESS;
  fclose(iniFile);
}

_SEKCIE* getAllSections(){
    if(sekcia!=NULL)return sekcia;
    return NULL;
}

int getSectionCount(){
  return sections;
}