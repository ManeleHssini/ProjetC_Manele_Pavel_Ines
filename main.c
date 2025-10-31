#include <stdio.h>
#include "bigbinary.h"

int main(void) {
    // --- Initialisations ---
    BigBinary a = initBigBinaryDepuisChaine("1010");   // 10
    BigBinary b = initBigBinaryDepuisChaine("0011");   // 3
    BigBinary z = initBigBinaryVide();                 // 0

    printf("a = "); afficheBigBinary(a);
    printf("b = "); afficheBigBinary(b);
    printf("z = "); afficheBigBinary(z);

    // --- Addition ---
    BigBinary s = additionBigBinary(&a, &b);           // 10 + 3 = 13 -> 1101
    printf("a + b = ");
    afficheBigBinary(s);

    // --- Soustraction (A >= B) ---
    // Ici a (10) >= b (3) : OK
    BigBinary d = soustractionBigBinary(&a, &b);       // 10 - 3 = 7 -> 0111
    printf("a - b = ");
    afficheBigBinary(d);

    // --- Division par 2 ---
    printf("a / 2 = ");
    divisePar2(&a);                                    // 10 / 2 = 5 -> 0101
    afficheBigBinary(a);

    // --- Comparaisons ---
    printf("a == b ? %d\n", Egal(&a, &b));
    printf("a <  b ? %d\n", Inferieur(&a, &b));
    printf("b <  s ? %d\n", Inferieur(&b, &s));
    printf("s == s ? %d\n", Egal(&s, &s));

    // --- Libération mémoire ---
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&z);
    libereBigBinary(&s);
    libereBigBinary(&d);

    return 0;
}
