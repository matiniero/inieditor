#ifndef __INI_PROJECT__
#define __INI_PROJECT__

#define MAX_LINE 512 /*max pocet znakov v riadku*/
#define MAX_SECS 128 /*max pocet sekcii*/

#define SEC_NAME 64 /*max dlzka nazvu sekcie*/
#define RECORD_KEY 64 /*max dlzka kluca v zazname*/
#define RECORD_VALUE 128 /*max dlzka hodnoty v zazname*/

struct Komentare {
  char komentar[MAX_LINE];
};

struct Zaznamy {
  char kluc[RECORD_KEY];
  char hodnota[RECORD_VALUE];
  struct Komentare *comments;
  int commentsCount;
};

struct Sekcie {
  char meno[SEC_NAME];
  struct Zaznamy *records;
  struct Komentare *comments;
  int recordsCount;
  int commentsCount;
};

typedef struct Zaznamy _ZAZNAMY;
typedef struct Sekcie _SEKCIE;
typedef struct Komentare _KOMENTARE;

/* načítanie INI dát zo súboru */
int nacitajSubor(char *filename);

/* uloženie INI dát do súboru */
int ulozSubor(char *filename);

/* odstránenie kľúča a hodnoty z danej sekcie */
int vymazZaznam(char *sekc, char *kluc);

/* získanie všetkých kľúčov zo všetkých sekcií: funkcia nech vráti vhodnú štruktúru obsahujúcu všetky kľúče */
_ZAZNAMY* getAllRecords();

/* získanie kľúčov z danej sekcie: funkcia nech vráti vhodnú štruktúru obsahujúcu všetky kľúče z danej sekcie */
_ZAZNAMY* getSectionRecords(char *sectionName);

/* pridanie kľúča a hodnoty do danej sekcie spôsobom, kde v prípade existencie daného kľúča sa hodnota prepíše */
int pridajZaznamPrepis(char *sekciameno, char *kluc, char *hodnota);

/* pridanie kľúča a hodnoty do danej sekcie spôsobom, kde v prípade existencie daného kľúča nastane chybový stav */
int pridajZaznamNeprepis(char *sekciameno, char *kluc, char *hodnota);

/* získanie hodnoty pre daný kľúč ako reťazca */
char* getStringValue(char *sekciameno, char *kluc);

/* získanie hodnoty pre daný kľúč ako celého čísla */
/* toto som neimplementoval lebo to je podla mna nezmysel */
int getIntValue(char *sekciameno, char *kluc);

_ZAZNAMY* getRecords(char *sectionName,char *recordKey);
int getRecordsCount(char *sectionName, char *recordKey);
void freememory();
void errorMsg(const char *text, int qValue);

_SEKCIE* getAllSections();
int getSectionCount();

#endif /* __INI_PROJECT__ */