#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class NimGame {
private:
    vector<int> piles;
    int currentPlayer;

public:
    NimGame(const vector<int>& initialPiles) : piles(initialPiles), currentPlayer(0) {}

    void displayBoard() {
        cout << "\n=== Game State ===" << endl;
        for (int i = 0; i < piles.size(); i++) {
            cout << "Pile " << i << ": ";
            for (int j = 0; j < piles[i]; j++) {
                cout << "* ";
            }
            cout << "(" << piles[i] << ")" << endl;
        }
        cout << endl;
    }

    bool isGameOver() {
        for (int pile : piles) {
            if (pile > 0) return false;
        }
        return true;
    }

    int calculateNimSum() {
        int nimSum = 0;
        for (int pile : piles) {
            nimSum ^= pile;
        }
        return nimSum;
    }

    bool makeMove(int pileIdx, int count) {
        if (pileIdx < 0 || pileIdx >= piles.size() || count <= 0 || count > piles[pileIdx]) {
            cout << "Invalid move!" << endl;
            return false;
        }

        piles[pileIdx] -= count;
        return true;
    }

    pair<int, int> getAIMove() {
        int nimSum = calculateNimSum();

        if (nimSum == 0) {
            for (int i = 0; i < piles.size(); i++) {
                if (piles[i] > 0) {
                    return {i, 1};
                }
            }
        }

        for (int i = 0; i < piles.size(); i++) {
            int newPile = piles[i] ^ nimSum;
            if (newPile < piles[i]) {
                return {i, piles[i] - newPile};
            }
        }

        for (int i = 0; i < piles.size(); i++) {
            if (piles[i] > 0) {
                return {i, 1};
            }
        }

        return {-1, -1};
    }

    void playerMove() {
        int pile, count;
        bool validMove = false;

        while (!validMove) {
            cout << "Player " << (currentPlayer + 1) << "'s turn" << endl;
            cout << "Enter pile index: ";
            cin >> pile;
            cout << "Enter number of items to take: ";
            cin >> count;

            validMove = makeMove(pile, count);
        }
    }

    void play(bool isHumanVsAI = false) {
        srand(time(0));

        cout << "=== Welcome to Nim Game ===" << endl;
        cout << "Rules: Take any number of items from one pile." << endl;
        cout << "The player who takes the last item wins!" << endl;

        while (!isGameOver()) {
            displayBoard();

            if (isHumanVsAI && currentPlayer == 1) {
                cout << "AI's turn..." << endl;
                auto [pileIdx, count] = getAIMove();
                cout << "AI takes " << count << " items from pile " << pileIdx << endl;
                makeMove(pileIdx, count);
            } else {
                playerMove();
            }

            currentPlayer = 1 - currentPlayer;
        }

        displayBoard();
        cout << "Player " << (currentPlayer + 1) << " wins!" << endl;
    }
};

int main() {
    vector<int> initialPiles = {3, 5, 7};

    cout << "Choose game mode:" << endl;
    cout << "1. Player vs Player" << endl;
    cout << "2. Player vs AI" << endl;
    cout << "Enter choice (1 or 2): ";

    int choice;
    cin >> choice;

    NimGame game(initialPiles);

    if (choice == 2) {
        game.play(true);
    } else {
        game.play(false);
    }

    return 0;
}
