#include<iostream>
#include<bits/stdc++.h>
#include<algorithm>
#include <random>

#include<vector>

#include<string>

#include<fstream>
using namespace std;


bool isPrime(int n)
{
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

long long nextPrime(long long N)
{
    while (!isPrime(N))
    {
        N++;
    }
    return N;
}
string generateRandomWord(long long length) {

    string word;

    for (long long i = 0; i < length; i++) {
        long long index = rand() % 26;
        char wordToBeAdded = 'a'+index ;
        word += wordToBeAdded;
    }

    return word;
}


void shuffleStrings(vector<string>& strings) {
    random_device rd;
    mt19937 g(rd());
    shuffle(strings.begin(), strings.end(), g);
}


const long long N = 10000, T = 0.1 * N;


struct Node
{
    string key;
    long long value;
    Node* next;
    bool deleted;
    Node(string k = "", long long v = -1, Node* n = nullptr) : key(k), value(v), next(n), deleted(false) {}
};




class ChainingHashTable
{
private:
    vector<Node*> table;
    int tableSize;
    int initialSize;
    long long size;
    int collisions;
    int probes;
    int maxChainLength;
    int insertionsSinceLastRehash;
    int deletionsSinceLastRehash;
    set<string> keys;
    int hashFunctionChoice; // 1 for h1, 2 for h2


    /*long long hash1(const string &key)
    {


         long long hashVal = 0;
        for (char ch : key)
        {

            hashVal = (37 * hashVal + ch) % tableSize;

        }

        return hashVal;
    }*/
     long long hash1(const string &key) const {
    unsigned long long hashVal = 5381; // Starting value, as per djb2
    for (char ch : key) {
        // hashVal * 33 + ch
        hashVal = ((hashVal << 5) + hashVal) + static_cast<unsigned long long>(ch);
    }
    return hashVal % tableSize;
}

    long long hash2(const string& key)
    {

        unsigned long long hashVal = 0;
        for (char ch : key) hashVal = (hashVal * 5 + ch) % tableSize;
        return hashVal;
    }

    long long hash(const string& key)
    {

        return hashFunctionChoice == 1 ? hash1(key) : hash2(key);
    }

    void reHash(bool increase)
    {
        long long newTableSize;
        if (increase)
        {
            newTableSize = nextPrime(tableSize * 2);
        }
        else
        {
            newTableSize = nextPrime(tableSize / 2);
        }

        if (newTableSize == tableSize) return;
        vector<Node*> newTable(newTableSize, nullptr);

        for (auto &head : table)
        {
            while (head != nullptr)
            {
                Node* next = head->next;
                tableSize = newTableSize;
                long long index = hash(head->key);
                head->next = newTable[index];
                newTable[index] = head;
                head = next;
            }
        }


table = move(newTable);


        tableSize = newTableSize;
    }


    int calculateMaxChainLength()
    {
        int maxChain = 0;
        for (auto head : table)
        {
            int chainLength = 0;
            while (head)
            {
                chainLength++;
                head = head->next;
            }
            maxChain = max(maxChain, chainLength);
        }
        return maxChain;
    }
public:
    ChainingHashTable(long long n, int hf,int maxChainLen) : initialSize(nextPrime(n)), tableSize(nextPrime(n)), size(0), collisions(0), probes(0), maxChainLength(maxChainLen), insertionsSinceLastRehash(0), deletionsSinceLastRehash(0), hashFunctionChoice(hf)
    {
        table.resize(tableSize, nullptr);

    }

    ~ChainingHashTable()
    {
        for (auto head : table)
        {
            while (head != nullptr)
            {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

long long getSize() const {
    return size;
}


int getProbes() const {
    return probes;
}


void resetProbes() {
    probes = 0;
}


int getCollisions() const {
    return collisions;
}


    pair<long long, int> findWithProbes(string key)
    {
        int localProbes = 0;
        long long index = hash(key);
        Node* current = table[index];
        while (current)
        {
            localProbes++;
            if (current->key == key)
            {
                return {current->value, localProbes};
            }
            current = current->next;
        }
        return {-1, localProbes};
    }

    double calculateAverageProbeCount()
    {
        if (keys.empty()) return 0.0;

        int sampleSize = max(1, static_cast<int>(keys.size() * 0.1)); // 10% of the keys
        vector<string> allKeys(keys.begin(), keys.end());
        random_device rd;
        mt19937 g(rd());
        shuffle(allKeys.begin(), allKeys.end(), g);

        int totalProbes = 0;
        for (int i = 0; i < sampleSize; ++i)
        {
            auto findResult = findWithProbes(allKeys[i]);
            totalProbes += findResult.second;
        }

        return static_cast<double>(totalProbes) / sampleSize;
    }
    void insert(string key, int value)
    {

        if (keys.insert(key).second) {

            long long index = hash(key);



            if (table[index] != nullptr)
            {
                collisions++;
            }

            Node* newNode = new Node(key, value, table[index]);
            table[index] = newNode;

            size++;
            insertionsSinceLastRehash++;

            if (insertionsSinceLastRehash % 100 == 0 && calculateMaxChainLength() > maxChainLength)
            {
                cout << "Before Rehash: Average probe count= "<<calculateAverageProbeCount()<<",Load Factor = " << static_cast<double>(size) / tableSize << ", Max Chain Length = " << calculateMaxChainLength() << endl;
                reHash(true);
                cout << "After Rehash: Average probe count= "<<calculateAverageProbeCount()<<", Load Factor = " << static_cast<double>(size) / tableSize << ", Max Chain Length = " << calculateMaxChainLength() << endl;
                insertionsSinceLastRehash = 0;
            }
        }}

    long long find(string key)
    {
        long long index = hash(key);
        Node* current = table[index];
        while (current)
        {
            probes++;
            if (current->key == key)
            {
                return current->value;
            }
            current = current->next;
        }
        return -1;
    }


    void Delete(string key)
    {
        long long index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current)
        {
            if (current->key == key)
            {
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    table[index] = current->next;
                }
                delete current;
                size--;
                deletionsSinceLastRehash++;
                keys.erase(key);

                if (deletionsSinceLastRehash % 100 == 0)
                {
                    int currentMaxChainLength = calculateMaxChainLength();
                    if (currentMaxChainLength < 0.8 * maxChainLength && tableSize/2 > initialSize)
                    {
                        cout << "Before Rehash: Average probe count= "<<calculateAverageProbeCount()<<", Load Factor = " << static_cast<double>(size) / tableSize << ", Max Chain Length = " << currentMaxChainLength << endl;
                        reHash(false);
                        cout << "After Rehash:  Average probe count= "<<calculateAverageProbeCount()<<", Load Factor = " << static_cast<double>(size) / tableSize << ", Max Chain Length = " << calculateMaxChainLength() << endl;
                    }
                    deletionsSinceLastRehash = 0;
                }

                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void setHashFunctionChoice(int hfChoice)
    {
        hashFunctionChoice = hfChoice;
    }

};





class ClosedHash
{
     struct Node {
        string key;
        long long value;
        bool deleted;
        Node(string k = "", long long v = -1, bool d = true) : key(k), value(v), deleted(d) {}
    };

    vector<Node> table;
    int hashFunctionChoice, probingType;
    long long maxSize, actualSize, collisions, probes;
    int insertionsSinceLastRehash;
    int deletionsSinceLastRehash;
    set<string> keys;
    const long long c1 = 37, c2 = 41;
    /*long long hash1(const string &key) const
    {
        unsigned long long hashVal = 0;
        for (char ch : key)
        {
            hashVal = (37 * hashVal + ch) % maxSize;
        }
        return hashVal;
    }*/
    long long hash1(const string &key) const {
    unsigned long long hashVal = 5381; // Starting value, as per djb2
    for (char ch : key) {
        // hashVal * 33 + ch
        hashVal = ((hashVal << 5) + hashVal) + static_cast<unsigned long long>(ch);
    }
    return hashVal % maxSize;
}


    long long hash2(const string& key)const
    {
        unsigned long long hashVal = 0;
        for (char ch : key) hashVal = (hashVal * 5 + ch) % maxSize;
        return hashVal;
    }

    long long auxHash(const string &key)const
    {

        long long hashVal = 0;
        for (char ch : key)
        {
            hashVal = 31 * hashVal + ch;
        }
        return hashVal % maxSize;
    }


    long long customProbe(const string &key, long long i) const
    {
        long long hash1Val = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
        long long hash2Val = auxHash(key);
        return (hash1Val + c1 * i *hash2Val+ c2 * i * i) % maxSize;
    }


    long long doubleHash(const string &key, long long i) const
    {
        long long hash1Val = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
        long long hash2Val = auxHash(key);
        if (hash2Val == 0) hash2Val = 1;
        return (hash1Val + i * hash2Val) % maxSize;
    }




public:
     ClosedHash(long long n, int hashFunctionChoice, int probingType) : maxSize(nextPrime(n)), actualSize(0), collisions(0), probes(0), hashFunctionChoice(hashFunctionChoice), probingType(probingType) {
        table.resize(maxSize);
    }

long long getSize() const {
    return actualSize;
}


int getProbes() const {
    return probes;
}


void resetProbes() {
    probes = 0;
}


int getCollisions() const {
    return collisions;
}
void resetCollition()
{
    collisions=0;
}


/*double calculateAverageProbeCount() {
    if (keys.empty()) return 0.0;


    //vector<string> keysVector(keys.begin(), keys.end());

    //random_device rd;
    //mt19937 g(rd());
    //shuffle(keysVector.begin(), keysVector.end(), g);



    int sampledSize = static_cast<int>(keys.size() * 0.1);
    sampledSize = max(1, sampledSize);
    double totalProbes = 0;
    for (int i = 0; i < sampledSize; ++i) {
            int index = rand()%maxSize;
            string key = keys[index];
    cout << " hello " << index << " "  << key<< endl;
        pair<long long, int> result = findWithProbes(key);
        totalProbes += result.second;
    }

    return totalProbes / sampledSize;
}*/

double calculateAverageProbeCount() {
    if (keys.empty()) return 0.0;

    int sampledSize = static_cast<int>(keys.size() * 0.1);
    sampledSize = max(1, sampledSize);

    double totalProbes = 0;
    for (int i = 0; i < sampledSize; ++i) {
        auto it = keys.begin();
        int randomStep = rand() % keys.size();
        advance(it, randomStep);

        string key = *it;


        pair<long long, int> result = findWithProbes(key);
        totalProbes += result.second;
    }

    return totalProbes / sampledSize;
}


pair<long long, int> findWithProbes(const string& key) {
    int probeCount = 0;
    long long index = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
    long long step = probingType == 2 ? customProbe(key, 0) : doubleHash(key, 0);
    for (long long i = 0; i < maxSize; ++i) {
        long long currentIndex = (index + i * step) % maxSize;
        probeCount++;
        if (table[currentIndex].key == key && !table[currentIndex].deleted) {
            return {table[currentIndex].value, probeCount};
        }
        else if (table[currentIndex].key.empty() && table[currentIndex].deleted) {
            return {-1, probeCount}; // Key not found
        }
    }
    return {-1, probeCount}; // Key not found after full cycle
}



void insert(const string& key, long long value) {
    if (keys.insert(key).second) { // Only insert if key is new
        long long index = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
        long long step = probingType == 2 ? customProbe(key, 0) : doubleHash(key, 0);

        bool inserted = false;
        for (long long i = 0; i < maxSize && !inserted; ++i) {
            long long currentIndex = (index + i * step) % maxSize;
            if (table[currentIndex].deleted || table[currentIndex].key.empty()) {
                table[currentIndex] = Node(key, value, false);
                actualSize++;
                inserted = true;
                probes++;
            } else if (table[currentIndex].key == key) {

                table[currentIndex].value = value;
                inserted = true;
            } else {

                collisions++;
                probes++;
            }
        }

    }

}

    long long find(const string& key) {
    long long index = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
    long long step = probingType == 2 ? customProbe(key, 0) : doubleHash(key, 0);
    for (long long i = 0; i < maxSize; ++i) {
        long long currentIndex = (index + i * step) % maxSize;
        if (table[currentIndex].key == key && !table[currentIndex].deleted) {
            return table[currentIndex].value;
        }
        else if (table[currentIndex].key.empty() && table[currentIndex].deleted) {
            break;
        }
    }
    return -1;
}



void remove(const string& key) {

    long long index = hashFunctionChoice == 1 ? hash1(key) : hash2(key);
    long long step = probingType == 2 ? customProbe(key, 0) : doubleHash(key, 0);

    for (long long i = 0; i < maxSize; ++i) {
        long long currentIndex = (index + i * step) % maxSize;
        if (table[currentIndex].key == key && !table[currentIndex].deleted) {
            table[currentIndex].deleted = true;
            actualSize--;
            deletionsSinceLastRehash++;
            keys.erase(key);

            return;
        }
    }

}



};

int main(){
    srand(2005);
    vector<string> randomWords(N);
    vector<string> testrandomWords(N);
    long long size[3] = {5000, 10000 , 20000};
    double report [3][3][2][2];

    for (long long i = 0; i < N; i++) {
        long long length = 5 + rand() % 6;
        randomWords[i] = generateRandomWord(length);
    }



     testrandomWords = randomWords; // Copy
    shuffleStrings(testrandomWords);

    long long n = 0;

    for(long long i : size){
        for(int j = 1; j <= 2; j++) {
            ChainingHashTable *openHash = new ChainingHashTable(i, j,16);

            for(long long m = 0; m < 0.5*i; m++){



                    openHash->insert(testrandomWords[m], openHash->getSize() + 1);
            }


            openHash->resetProbes();

            for(long long m = 0; m < T; m++)
                openHash->find(testrandomWords[m]);

            report[n][0][j - 1][0] = openHash->getCollisions();
            report[n][0][j - 1][1] = (double) openHash->getProbes() / T;

            delete openHash;
        }
       // cout<<"thank you"<<endl;

        for(int j = 1; j <= 2; j++) {
            for (int k = 2; k >= 1; k--) {

                ClosedHash *closedHash = new ClosedHash(i,j,k);


                for(long long m = 0; m < 0.5*i; m++){
                    if(closedHash->find(randomWords[m]) == -1)
                        closedHash->insert(randomWords[m], closedHash->getSize() + 1);
                }


                closedHash->resetProbes();

                for(long long m = 0; m < T; m++)
                    closedHash->find(testrandomWords[m]);


                report[n][k ][j - 1][0] = closedHash->getCollisions();

                report[n][k][j - 1][1] = (double) closedHash->calculateAverageProbeCount() ;

closedHash->resetCollition();
                delete closedHash;

            }
        }
        n++;
    }


ofstream file("report1.txt");
    file << "______________________________________________________________________\n";
    file << "| Hash       | Collision         | Hash Function 1 | Hash Function 2 |\n";
    file << "| Table      | Resolution        |___________________________________|\n";
    file << "| Size       | method            | #of    | Avg.   | #of    | Avg.   |\n";
    file << "|            |                   | Colls. | Probes | Colls. | Probes |\n";
    file << "|____________________________________________________________________|\n";

for (int i = 0; i < 3; ++i) {
        file << "| N = " << std::setw(7) << size[i];

        for (int j = 0; j < 3; ++j) {
            std::string method;
            switch (j) {
                case 0: method = "Separate Chaining"; break;
                case 1: method = "           |  Double Hashing"; break;
                case 2: method = "           |  Custom Probing"; break;
            }
            if(j==0)
            file << "| " << method <<setw(19 - method.length()) << " | ";
            else
                file << "| " << method << setw(19 - method.length()) << "   | ";

            for (int k = 0; k < 2; ++k) {
                for (int m = 0; m < 2; ++m) {
                    file << std::setw(5) << report[i][j][k][m] << "  | ";
                }
            }
            file << "\n";
            if (j == 2) {
                file << "|____________|___________________|________|________|________|________|\n";
            } else {
                file << "|            |___________________|________|________|________|________|\n";
            }
        }
}
    file.close();

    return 0;
}

