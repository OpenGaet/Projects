#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "rngs.h"


void confirm (int passed, char* pass, char* fail) {
	if (passed == 1)
		printf("%s\n", pass);
	else
		printf("%s\n", fail);
}


int checkBaron (int p, struct gameState *post) {
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));

    int r;
    printf ("drawCard PRE: p %d HC %d DeC %d DiC %d\n", p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);

    r = effectBaron (p, post);

    printf ("drawCard POST: p %d HC %d DeC %d DiC %d\n", p, post->handCount[p], post->deckCount[p], post->discardCount[p]);

    // baron should increase number of buys by one
    pre->numBuys++;

    // If player has estate, discard and gain 4 gold
    if {

    }
    // otherwise gain estate
    else {

    }

    if (pre.deckCount[p] > 0) {
        pre.handCount[p]++;
        pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
        pre.deckCount[p]--;
    }
    else if (pre.discardCount[p] > 0) {
        memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
        memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
        pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
        pre.handCount[p]++;
        pre.deckCount[p] = pre.discardCount[p]-1;
        pre.discardCount[p] = 0;
    }

    assert (r == 0);

    assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
}


int main () {

    int i, n, r, p, deckCount, discardCount, handCount;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    struct gameState G;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

    printf ("Testing effectBaron().\n");

    printf ("RANDOM TESTS.\n");

    SelectStream(2);
    PutSeed(3);

    for (n = 0; n < 2000; n++) {
        for (i = 0; i < sizeof(struct gameState); i++) {
            ((char*)&G)[i] = floor(Random() * 256);
        }
        p = floor(Random() * 2);
        G.deckCount[p] = floor(Random() * MAX_DECK);
        G.discardCount[p] = floor(Random() * MAX_DECK);
        G.handCount[p] = floor(Random() * MAX_HAND);
        checkBaron(p, &G);
    }

    printf ("ALL TESTS OK\n");

    exit(0);

}
