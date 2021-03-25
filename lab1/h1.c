/********* h1.c **********/ 
#include <stdio.h>
#include <string.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

struct partition {
	u8 drive;             /* drive number FD=0, HD=0x80, etc. */

	u8  head;             /* starting head */
	u8  sector;           /* starting sector */
	u8  cylinder;         /* starting cylinder */

	u8  sys_type;         /* partition type: NTFS, LINUX, etc. */

	u8  end_head;         /* end head */
	u8  end_sector;       /* end sector */
	u8  end_cylinder;     /* end cylinder */

	u32 start_sector;     /* starting sector counting from 0 */
	u32 nr_sectors;       /* number of of sectors in partition */
};

typedef struct person{
  char name[64];
  int  id;
  int  age;
  char gender;
}PERSON;

PERSON kcw, *p;

int main()
{

// Access struct fields by . operator: OK but ugly
   kcw.id = 12345678;
   kcw.age = 83;
   kcw.gender = 'M';
  
   p = &kcw;

// Deference pointer to struct, then use . operator: NOT GOOD either!
   (*p).id = 123;
   (*p).age = 120;

// Use pointer by -> operator is the BEST WAY:
   p->id = 12345678;
   p->age = 83;
   p->gender = 'M';
   strcpy(p->name, "k.c. wang");

   printf("name=%s id=%d age=%d gender=%c\n",
	 p->name, p->id, p->age, p->gender);
}