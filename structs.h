typedef struct head {
	unsigned char type; //tipul de date stocate
	unsigned int len;   //lungimea totala
} head; //structura header
 
typedef struct data_structure {
	head *header; //header-ul datelor
	void *data;   //datele efective
} data_structure; //structura de pasare a datelor