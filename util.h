#ifndef UTIL_H
#define UTIL_H
#define DELETEARRAY(x) if(x != 0){delete[] x;x = 0;}
#define DELETE(x) if(x != 0){delete x;x = 0;}
#define FINALLY(x) catch(...){ x; throw; } x
 #define DOCLOSE(obj) if(obj!=0){ obj->close();}


#endif // UTIL_H
