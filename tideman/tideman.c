#include <cs50.h>
#include <stdio.h>
#include <string.h>



// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];


// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(pair pr[], int a, int b);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    //initialize preferences 0
    for (int i = 0; i < MAX; i++)
    {

        for (int j = 0; j < MAX; j++)
        {

            preferences[i][j] = 0;

        }

    }


    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = i + 1; j < candidate_count; j++)
        {

            preferences[ranks[i]][ranks[j]] += 1;

        }

    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = 0; j < candidate_count; j++)
        {

            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                k++;
                pair_count += 1;

            }
            else if (preferences[i][j] < preferences[j][i])
            {

                pairs[k].winner = j;
                pairs[k].loser = i;
                k++;
                pair_count += 1;
            }
        }
    }
    pair_count /= 2;
    return;
}

// Sort pairs in decreasing order by strength of victory
void swap(pair pr[], int a, int b)
{
    pair tmp;
    tmp.winner = pr[a].winner;
    tmp.loser = pr[a].loser;

    pr[a].winner = pr[b].winner;
    pr[a].loser = pr[b].loser;

    pr[b].winner = tmp.winner;
    pr[b].loser = tmp.loser;
    return;

}
void sort_pairs(void)
{
    // TODO
    //pair tmp[MAX * (MAX - 1) / 2];
    int swapindex = 1;

    while (swapindex)
    {
        swapindex = 0;

        for (int i = 0; i < pair_count - 1; i++)
        {
            int one, two;

            one = preferences[pairs[i].winner][pairs[i].loser];
            two = preferences[pairs[i + 1].winner][pairs[i + 1].loser];
            // the victory strength is the difference between the winner & loser for each

            if (one < two)
            {
                swap(pairs, i, i + 1);
                swapindex++;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int a, b;
    for (int i = 0; i < (MAX * (MAX - 1) / 2); i++)
    {
        a = pairs[i].winner;
        b = pairs[i].loser;

        locked[a][b] = true;

        if (i != 0 && b == pairs[0].winner)
        {
            locked[a][b] = false;
        }

    }


    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int flag = 0;
    int holder;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (locked[i][j] == false)
            {
                break;

            }
            else
            {

                holder = i;
            }
        }
        flag = 0;
    }


    printf("%s\n", candidates[holder]);
    return;
}

