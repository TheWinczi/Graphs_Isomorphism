#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class Node
{
public:
    int key;
    Node* left; // children
    Node* right; // siblings
    Node* parent; // parents
    bool empty;
    Node(int key)
        : key(key), left(NULL), right(NULL), parent(NULL), empty(false) {};
};

class NodeOfTree
{
public:
    NodeOfTree* next;
    NodeOfTree* previous;
    Node* treeNode;

    NodeOfTree(Node* treeNode)
        : next(NULL), previous(NULL), treeNode(treeNode) {};
};
class List
{
public:
    NodeOfTree* head;
    NodeOfTree* tail;

    List()
        : head(NULL), tail(NULL) {};

    void push_back(Node* node)
    {
        NodeOfTree* temp = new NodeOfTree(node);

        if (this->head == NULL)
        {
            this->head = temp;
            this->tail = temp;
        }
        else
        {
            temp->previous = this->tail;
            this->tail->next = temp;
            this->tail = this->tail->next;
        }
    }
};


class DataTree
{

private:
    Node* root;
    int howManyNumbers;
    int howManyEmptyNodes;
    List listOfNodes;

public:
    DataTree()
        : root(NULL), howManyNumbers(0), howManyEmptyNodes(0) {};

    void add_sibling(int key, Node* node)
    {
        Node* tmp = node;
        while (tmp->right)
            tmp = tmp->right;

        Node* newNode = new Node(key);

        newNode->parent = tmp->parent;
        tmp->right = newNode;

        this->howManyNumbers++;
    }

    void add_child(int key, Node* node)
    {
        Node* newNode = new Node(key);

        newNode->parent = node;
        node->left = newNode;

        this->howManyNumbers++;
    }

    void add_empty_child(Node* node)
    {
        Node* emptyNode = new Node((-1) * howManyEmptyNodes);
        emptyNode->empty = true;

        if (this->root == NULL)
            this->root = emptyNode;
        else
        {
            emptyNode->parent = node;
            node->left = emptyNode;
        }

        listOfNodes.push_back(emptyNode);
        this->howManyEmptyNodes++;
    }

    void add_empty_sibling(Node* node)
    {
        Node* emptyNode = new Node((-1) * howManyEmptyNodes);
        emptyNode->empty = true;

        node->right = emptyNode;
        emptyNode->parent = node->parent;

        this->howManyEmptyNodes++;
        listOfNodes.push_back(emptyNode);
    }

    Node* get_root() { return root; }
    int const get_how_many_numbers() { return this->howManyNumbers; }
    int const get_how_many_empty_nodes() { return this->howManyEmptyNodes; }
    NodeOfTree* get_head_of_list() { return listOfNodes.head; }
    NodeOfTree* get_tail_of_list() { return listOfNodes.tail; }
};

void parse_input(string inputLine, DataTree& myTree);
void analize_numbers(DataTree firstTree, DataTree secondTree, int firstArray[], int secondArray[], int* destArray[]);
void increment_horizontal(DataTree tree, Node* node, int array[], int* destArray[], int poprzednik);
void increment_vertical(DataTree tree, Node* node, int array[], int* destArray[], int poprzednik);
void analize_nodes(DataTree firstTree, DataTree secondTree, int* destArray[], int firstarray[], int secondArray[]);
void combinations(DataTree trees[], int howManyTrees);
void reset_array(int* array[], int rows, int columns);
void write_array(int* array[], int rows, int columns);
void fill_array(DataTree tree, int array[]);

int main()
{

    int howManyTrees;
    scanf("%d\n", &howManyTrees);

    DataTree* myTrees = new DataTree[howManyTrees];
    string line;
    for (int i = 0; i < howManyTrees; i++)
    {
        getline(cin, line);
        parse_input(line, myTrees[i]);
    }

    combinations(myTrees, howManyTrees);
    delete[] myTrees;
    return 0;
}

void combinations(DataTree trees[], int howManyTrees)
{
    for (int i = 0; i < howManyTrees; i++)
    {
        int howManyRows = trees[i].get_how_many_empty_nodes() + trees[i].get_how_many_numbers();
        int* firstArray = new int[howManyRows];
        fill_array(trees[i], firstArray);

        for (int j = i + 1; j < howManyTrees; j++)
        {
            int howManyColumns = trees[j].get_how_many_empty_nodes() + trees[j].get_how_many_numbers();
            int* secondArray = new int[howManyColumns];
            fill_array(trees[j], secondArray);

            int** arrays = new int* [howManyRows];
            for (int k = 0; k < howManyRows; k++)
                arrays[k] = new int[howManyColumns];
            reset_array(arrays, howManyRows, howManyColumns);

            analize_numbers(trees[i], trees[j], firstArray, secondArray, arrays);
            analize_nodes(trees[i], trees[j], arrays, firstArray, secondArray);

            int wynik = trees[i].get_how_many_numbers() - (arrays[trees[i].get_how_many_numbers()][trees[j].get_how_many_numbers()]);

            printf("%d\n", wynik);
            delete[] secondArray;

            for (int k = 0; k < howManyRows; k++)
                delete[] arrays[k];
            delete[] arrays;
        }
        delete[] firstArray;
    }
}

int power(int base, int exponent)
{
    if (exponent > 0)
        return base * power(base, exponent - 1);
    else
        return 1;
}

void maxi(int* array[], int columns, int rows, int& max, int index, int jakDuzoPop, int* poprzednicy[])
{
    if (index == 0)
    {
        bool flag = true;
        for (int k = 1; k < jakDuzoPop; k++)
        {
            for (int l = k + 1; l < jakDuzoPop; l++)
                if (poprzednicy[l][1] == poprzednicy[k][1] || poprzednicy[l][0] == poprzednicy[k][0])
                {
                    flag = false;
                    break;
                }

            if (flag == false)
                break;
        }
        if (!flag)
            return;
    }

    for (int i = 0; i < rows; i++)
    {
        poprzednicy[index][0] = i;

        for (int j = 0; j < columns; j++)
        {
            poprzednicy[index][1] = j;

            if (array[i][j] == 0)
                continue;

            if (index > 0)
                maxi(array, columns, rows, max, index - 1, jakDuzoPop, poprzednicy);
            else if (index == 0)
            {
                bool flag = true;
                for (int l = 1; l < jakDuzoPop; l++)
                    if (poprzednicy[index][1] == poprzednicy[l][1] || poprzednicy[index][0] == poprzednicy[l][0])
                    {
                        flag = false;
                        break;
                    }

                if (flag)
                {
                    int sum = 0;
                    for (int k = 0; k < jakDuzoPop; k++)
                        sum += array[poprzednicy[k][0]][poprzednicy[k][1]];

                    if (sum > max)
                        max = sum;
                }
            }
        }
    }
}

void analize_nodes(DataTree firstTree, DataTree secondTree, int* destArray[], int rowsArray[], int columnsArray[])
{
    NodeOfTree* firstPointer = firstTree.get_tail_of_list();
    Node* node;

    int rows = 0;
    int columns = 0;
    int indexY = firstTree.get_how_many_empty_nodes() + firstTree.get_how_many_numbers() - 1;

    while (firstPointer)
    {
        int indexX = secondTree.get_how_many_empty_nodes() + secondTree.get_how_many_numbers() - 1;

        node = firstPointer->treeNode->left;
        while (node)// ustal ilosc wierszy
        {
            rows++;
            node = node->right;
        }

        NodeOfTree* secPointer = secondTree.get_tail_of_list();
        while (secPointer)
        {
            node = secPointer->treeNode->left;
            while (node)// ustal ilosc kolumn
            {
                columns++;
                node = node->right;
            }

            int** array = new int* [rows];
            for (int i = 0; i < rows; i++)
                array[i] = new int[columns];

            int rowIndex = 0, columnIndex = 0, yIndex = 0;
            node = firstPointer->treeNode->left;
            while (node)
            {
                int l = 0;
                for (rowIndex = 0; rowIndex < firstTree.get_how_many_numbers() + firstTree.get_how_many_empty_nodes(); rowIndex++)
                    if (node->key == rowsArray[rowIndex])
                        break;

                Node* node2 = secPointer->treeNode->left;
                while (node2)
                {
                    for (columnIndex = 0; columnIndex < secondTree.get_how_many_numbers() + secondTree.get_how_many_empty_nodes(); columnIndex++)
                        if (node2->key == columnsArray[columnIndex])
                            break;

                    array[yIndex][l] = destArray[rowIndex][columnIndex];
                    l++;
                    node2 = node2->right;
                }

                yIndex++;
                node = node->right;
            }

            int tmpColumns = columns;
            int tmpRows = rows;
            for (int i = tmpRows - 1; i >= 0; i--)
            {
                bool flag = true;
                for (int j = 0; j < columns; j++)
                    if (array[i][j] != 0)
                    {
                        flag = false;
                        break;
                    }

                if (flag)
                {
                    for (int k = i; k < tmpRows - 1; k++)
                        for (int l = 0; l < tmpColumns; l++)
                            array[k][l] = array[k + 1][l];

                    tmpRows--;
                }
            }
            for (int j = tmpColumns - 1; j >= 0; j--)
            {
                bool flag = true;
                for (int i = 0; i < rows; i++)
                    if (array[i][j] != 0)
                    {
                        flag = false;
                        break;
                    }

                if (flag)
                {
                    for (int k = j; k < tmpColumns - 1; k++)
                        for (int l = 0; l < tmpRows; l++)
                            array[l][k] = array[l][k + 1];

                    tmpColumns--;
                }
            }

            int max = 0;
            int howMany = (tmpRows <= tmpColumns ? tmpRows : tmpColumns);
            for (int j = 1; j <= howMany; j++)
            {
                int** pop = new int* [j];

                for (int i = 0; i < j; i++)
                {
                    pop[i] = new int[2];
                    pop[i][0] = pop[i][1] = 0;
                }

                maxi(array, tmpColumns, tmpRows, max, j - 1, j, pop);

                for (int i = 0; i < j; i++)
                    delete[] pop[i];
                delete[] pop;
            }

            int num = firstPointer->treeNode->key;
            for (rowIndex = 0; rowIndex < firstTree.get_how_many_numbers() + firstTree.get_how_many_empty_nodes(); rowIndex++)
                if (num == rowsArray[rowIndex])
                    break;

            node = secPointer->treeNode->left;
            while (node)
            {
                for (columnIndex = 0; columnIndex < secondTree.get_how_many_numbers() + secondTree.get_how_many_empty_nodes(); columnIndex++)
                    if (node->key == columnsArray[columnIndex])
                    {
                        if (destArray[rowIndex][columnIndex] > max)
                            max = destArray[rowIndex][columnIndex];

                        break;
                    }

                node = node->right;
            }

            num = secPointer->treeNode->key;
            for (columnIndex = 0; columnIndex < secondTree.get_how_many_numbers() + secondTree.get_how_many_empty_nodes(); columnIndex++)
                if (num == columnsArray[columnIndex])
                    break;

            node = firstPointer->treeNode->left;
            while (node)
            {
                for (rowIndex = 0; rowIndex < firstTree.get_how_many_numbers() + firstTree.get_how_many_empty_nodes(); rowIndex++)
                    if (node->key == rowsArray[rowIndex])
                    {
                        if (destArray[rowIndex][columnIndex] > max)
                            max = destArray[rowIndex][columnIndex];

                        break;
                    }

                node = node->right;
            }

            destArray[indexY][indexX] = max;
            secPointer = secPointer->previous;
            columns = 0;
            indexX--;

            for (int i = 0; i < rows; i++)
                delete[] array[i];

            delete[] array;
        }

        firstPointer = firstPointer->previous;
        rows = 0;
        indexY--;
    }
}


int index;
void analize_numbers(DataTree firstTree, DataTree secondTree, int firstArray[], int secondArray[], int* destArray[])
{
    for (int i = 0; i < firstTree.get_how_many_numbers(); i++)
        for (int j = 0; j < secondTree.get_how_many_numbers(); j++)
            if (firstArray[i] == secondArray[j])
            {
                destArray[i][j]++;
                break;
            }

    index = firstTree.get_how_many_numbers();
    NodeOfTree* tNode = firstTree.get_head_of_list();
    while (tNode)
    {
        if (tNode->treeNode->left)
            increment_vertical(firstTree, tNode->treeNode->left, secondArray, destArray, tNode->treeNode->key);

        index++;
        tNode = tNode->next;
    }

    index = secondTree.get_how_many_numbers();
    tNode = secondTree.get_head_of_list();
    while (tNode)
    {
        if (tNode->treeNode->left)
            increment_horizontal(secondTree, tNode->treeNode->left, firstArray, destArray, tNode->treeNode->key);

        index++;
        tNode = tNode->next;
    }
}

void increment_horizontal(DataTree tree, Node* node, int array[], int* destArray[], int poprzednik)
{
    if (!node->empty)
    {
        for (int i = 0; i < tree.get_how_many_numbers(); i++)
            if (array[i] == node->key)
            {
                destArray[i][index]++;
                break;
            }
    }

    if (node->empty)
        poprzednik = node->key;

    if (node->left != NULL && node->key <= poprzednik)
        increment_horizontal(tree, node->left, array, destArray, poprzednik);
    if (node->right != NULL)
        increment_horizontal(tree, node->right, array, destArray, poprzednik);
}


void increment_vertical(DataTree tree, Node* node, int array[], int* destArray[], int poprzednik)
{
    if (!node->empty)
    {
        for (int i = 0; i < tree.get_how_many_numbers(); i++)
            if (array[i] == node->key)
            {
                destArray[index][i]++;
                break;
            }
    }

    if (node->empty)
        poprzednik = node->key;

    if (node->left != NULL && node->key <= poprzednik)
        increment_vertical(tree, node->left, array, destArray, node->key);
    if (node->right != NULL)
        increment_vertical(tree, node->right, array, destArray, node->key);
}

void parse_input(string inputLine, DataTree& myTree)
{
    Node* whereAmI = myTree.get_root();

    bool willAddChild = false;
    bool willAddSibling = false;
    bool firstNumber = true;

    for (int i = 0; i < inputLine.length(); i++)
    {
        if (inputLine[i] == '(')// dodaj jako dziecko
        {
            if (willAddChild || i == 0)
            {
                myTree.add_empty_child(whereAmI);

                if (whereAmI == NULL)
                    whereAmI = myTree.get_root();
                else
                    whereAmI = whereAmI->left;
            }
            else if (willAddSibling)
            {
                myTree.add_empty_sibling(whereAmI);
                whereAmI = whereAmI->right;
                willAddSibling = false;
            }

            willAddChild = true;
        }
        else if (inputLine[i] == ',')// dodaj jako brata
            willAddSibling = true;
        else if (inputLine[i] == ')')
        {
            if (whereAmI->parent != NULL)
                whereAmI = whereAmI->parent;
        }
        else if (inputLine[i] >= '0' && inputLine[i] <= '9') // Na wejsciu jest liczba
        {
            string strNumber = "";
            int number = 0;
            int wyk = 0;

            while (inputLine[i] >= '0' && inputLine[i] <= '9')
            {
                strNumber += inputLine[i];
                i++;
            }

            for (int j = strNumber.length() - 1; j >= 0; j--)
            {
                number += ((int)strNumber[j] - 48) * power(10, wyk);
                wyk++;
            }

            if (willAddChild)
            {
                myTree.add_child(number, whereAmI);
                whereAmI = whereAmI->left;
                willAddChild = false;
            }
            else
            {
                myTree.add_sibling(number, whereAmI);
                whereAmI = whereAmI->right;
            }
            i--;
        }
        else if (inputLine[i] == ';')
            break;
    }
}

void reset_array(int* array[], int rows, int columns)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            array[i][j] = 0;
}

void fill_array(DataTree tree, int array[])
{
    int index = 0;
    NodeOfTree* tmp = tree.get_head_of_list();

    while (tmp)
    {
        Node* tmp2 = tmp->treeNode->left;
        while (tmp2)
        {
            if (!tmp2->empty)
            {
                array[index] = tmp2->key;
                index++;
            }
            tmp2 = tmp2->right;
        }
        tmp = tmp->next;
    }

    tmp = tree.get_head_of_list();
    while (tmp)
    {
        array[index] = tmp->treeNode->key;
        tmp = tmp->next;
        index++;
    }
}
