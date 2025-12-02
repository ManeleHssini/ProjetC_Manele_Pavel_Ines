#include <stdio.h>
#include "bigbinary.h"
#include "rsa.h"

void printSeparator(const char *title) {
    printf("\n====================================\n");
    printf("        %s\n", title);
    printf("====================================\n");
}

int main(void) {

    /* ============================================================
                     PHASE 1 : TESTS DE BASE
       ============================================================ */
    printSeparator("TESTS PHASE 1");

    // Tests init
    BigBinary a = initBigBinaryDepuisChaine("1010");   // 10
    BigBinary b = initBigBinaryDepuisChaine("0011");   // 3
    BigBinary z = initBigBinaryVide();                 // 0

    printf("a = "); afficheBigBinary(a);
    printf("b = "); afficheBigBinary(b);
    printf("z = "); afficheBigBinary(z);

    // Addition
    BigBinary s = additionBigBinary(&a, &b); // 13
    printf("a + b = ");
    afficheBigBinary(s);

    // Soustraction
    BigBinary d = soustractionBigBinary(&a, &b); // 7
    printf("a - b = ");
    afficheBigBinary(d);

    // Division par 2
    printf("a / 2 = ");
    divisePar2(&a);
    afficheBigBinary(a);

    // Comparaisons
    printf("a == b ? %d\n", Egal(&a, &b));
    printf("a <  b ? %d\n", Inferieur(&a, &b));
    printf("b <  s ? %d\n", Inferieur(&b, &s));
    printf("s == s ? %d\n", Egal(&s, &s));

    // Libération Phase 1
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&z);
    libereBigBinary(&s);
    libereBigBinary(&d);

    /* ============================================================
                     PHASE 2 : PGCD
       ============================================================ */
    printSeparator("TEST PGCD (PHASE 2)");

    BigBinary A = initBigBinaryDepuisChaine("1010011"); // 83
    BigBinary B = initBigBinaryDepuisChaine("111001");  // 57

    printf("A = "); afficheBigBinary(A);
    printf("B = "); afficheBigBinary(B);

    BigBinary G = BigBinary_PGCD(A, B);
    printf("PGCD(A,B) = ");
    afficheBigBinary(G);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&G);

    /* ============================================================
                     PHASE 2 : MULTIPLICATION
       ============================================================ */
    printSeparator("TEST MULTIPLICATION");

    BigBinary m1 = initBigBinaryDepuisChaine("1011"); // 11
    BigBinary m2 = initBigBinaryDepuisChaine("110");  // 6

    printf("m1 = "); afficheBigBinary(m1);
    printf("m2 = "); afficheBigBinary(m2);

    BigBinary M = BigBinary_multiply(&m1, &m2); // 66
    printf("m1 * m2 = ");
    afficheBigBinary(M);

    libereBigBinary(&m1);
    libereBigBinary(&m2);
    libereBigBinary(&M);

    /* ============================================================
                     PHASE 2 : TEST SOUSTRACTION (DEBUG)
       ============================================================ */
    printSeparator("TEST SOUSTRACTION 21 - 6");

    BigBinary X = initBigBinaryDepuisChaine("10101"); // 21
    BigBinary Y = initBigBinaryDepuisChaine("00110"); // 6
    BigBinary Rsub = soustractionBigBinary(&X, &Y);

    printf("21 - 6 = ");
    afficheBigBinary(Rsub);

    libereBigBinary(&X);
    libereBigBinary(&Y);
    libereBigBinary(&Rsub);

    /* ============================================================
                     PHASE 2 : TEST MODULO
       ============================================================ */
    printSeparator("TEST MODULO");

    BigBinary mo1 = initBigBinaryDepuisChaine("10101"); // 21
    BigBinary mo2 = initBigBinaryDepuisChaine("00110"); // 6

    BigBinary R1 = BigBinary_mod(mo1, mo2);
    printf("21 mod 6 = ");
    afficheBigBinary(R1);

    libereBigBinary(&mo1);
    libereBigBinary(&mo2);
    libereBigBinary(&R1);

    /* ============================================================
                     PHASE 2 : MULTIPLICATION EGYPTIENNE
       ============================================================ */
    printSeparator("TEST MULTIPLICATION EGYPTIENNE");

    BigBinary e1 = initBigBinaryDepuisChaine("1101"); // 13
    BigBinary e2 = initBigBinaryDepuisChaine("1011"); // 11

    BigBinary R_egy = BigBinary_multiplyEgypt(&e1, &e2);
    printf("13 * 11 = ");
    afficheBigBinary(R_egy);

    libereBigBinary(&e1);
    libereBigBinary(&e2);
    libereBigBinary(&R_egy);

    /* ============================================================
                     PHASE 2 : EXPONENTIATION MODULAIRE
       ============================================================ */
    printSeparator("TEST EXPONENTIATION MODULAIRE");

    BigBinary base = initBigBinaryDepuisChaine("101");    // 5
    BigBinary mod  = initBigBinaryDepuisChaine("1101");   // 13

    BigBinary res = BigBinary_expMod(base, 3, mod);
    printf("5^3 mod 13 = ");
    afficheBigBinary(res); // doit être 1000

    libereBigBinary(&base);
    libereBigBinary(&mod);
    libereBigBinary(&res);

    /* ============================================================
                     PHASE 3 : RSA
       ============================================================ */
    printSeparator("TEST RSA");

    BigBinary p = initBigBinaryDepuisChaine("1011");   // 11
    BigBinary q = initBigBinaryDepuisChaine("1101");   // 13
    BigBinary e = initBigBinaryDepuisChaine("111");    // 7

    RSAKey key = RSA_generateKey(p, q, e);

    printf("n = "); afficheBigBinary(key.n);
    printf("e = "); afficheBigBinary(key.e);
    printf("d = "); afficheBigBinary(key.d);

    BigBinary message = initBigBinaryDepuisChaine("1001"); // 9

    BigBinary C = RSA_chiffre(message, key);
    printf("Chiffré = ");
    afficheBigBinary(C);

    BigBinary D = RSA_dechiffre(C, key);
    printf("Déchiffré = ");
    afficheBigBinary(D);

    /* ============================================================
                     FIN DU PROGRAMME
       ============================================================ */
    printSeparator("TOUS LES TESTS SONT FINIS");

    return 0;
}
