#pragma once
#include <iostream>

enum class Color {
    RED = 0, BLACK = 1
};
enum class Order {
    ASC = 0, DES = 1
};

template < typename keyType >
class RBtree {
private:
    struct RBnode {
        Color nodeColor;
        keyType key; //Dane zawarte w w�le
        RBnode* parent;
        RBnode* left;
        RBnode* right;
    };
    RBnode* root;
    RBnode* const NIL;
    unsigned numRedNodes;
    unsigned numBlackNodes;
    void transplant(RBnode* const, RBnode* const);
    RBnode* search(RBnode*,const keyType&) const;
    RBnode* minimum(RBnode*) const;
    void leftRotate(RBnode* const);
    void rightRotate(RBnode* const);
    void insertFixup(RBnode*);
    void deleteFixup(RBnode*);
    void RB_insert(RBnode*);
    void RB_delete(RBnode*);
    int maximum(const int, const int) const;
    int subtreeHeight(const RBnode* const) const;
    void copyTree(RBnode*, RBnode*, RBnode*);
    void ASC(const RBnode* const) const;
    void DES(const RBnode* const) const;

public:
    //Konstruktor
    RBtree();
    //Konstruktor kopii
    RBtree(const RBtree&);
    //Destruktor
    ~RBtree();
    void add(const keyType x);
    bool HasKey(const keyType x) const;
    bool isEmpty() const;
    int Height() const;
    void destroyTree();
    void displayTree(const Order) const;

    //Przeci��one operatory
    RBtree < keyType >& operator = (const RBtree < keyType >&);
    RBtree < keyType > operator + (const RBtree < keyType >&) const;
    RBtree < keyType >& operator += (const RBtree < keyType >&);
    bool operator == (const RBtree&) const;
    bool operator != (const RBtree&) const;
};

//Zast�puje jedno drzewo podrz�dne jako dziecko swojego rodzica innym drzewem podrz�dnym.
//Zast�puje drzewo podrz�dne zakorzenione w w�le u drzewem podrz�dnym zakorzenionym w w�le v
//wskaznik na wezel do wymiany
//wskaznik na wezel zast. v w drzewie
template < typename keyType >
void RBtree < keyType > ::transplant(RBnode* const u, RBnode* const v) {
    //Sprawdza, czy chcemy przeszczepi� korze�, lewe dziecko czy prawe dziecko
    if (u->parent == NIL) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

//Szuka drzewa dla okre�lonej warto�ci klucza. Zwraca NIL, je�li warto�� nie zosta�a znaleziona.
//A wska�nik u�ywany do w�z�a w drzewie. Wska�nik ten przemierza przeszukiwane drzewo.
//Warto�� szukana w drzewie.
//A wska�nik do w�z�a zawieraj�cego okre�lon� warto�� klucza.
template < typename keyType >
typename RBtree < keyType > ::RBnode* RBtree < keyType > ::search(RBnode* traverse, const keyType& keyValue) const {
    while (traverse != NIL && keyValue != traverse->key) {
        if (keyValue < traverse->key) {
            traverse = traverse->left;
        }
        else {
            traverse = traverse->right;
        }
    }
    return traverse;
}

template < typename keyType >
typename RBtree < keyType > ::RBnode* RBtree < keyType > ::minimum(RBnode* traverse) const {
    while (traverse->left != NIL) {
        traverse = traverse->left;
    }

    return traverse;
}

//UWAGA: Je�li prawe dziecko osi obrotu jest NIL, zostanie zg�oszony wyj�tek.
//Wykonuje obr�t w lewo wok� w�z�a obrotowego. Zak�ada, �e prawe dziecko punktu obrotu nie jest NIL.
//Zmieniaj� si� tylko wska�niki do w�z��w, nie ma to wp�ywu na inne atrybuty.
//Prawe dziecko w�z�a obrotowego przyjmuje jego pozycj� w drzewie. Pivot staje si� lewym dzieckiem swojego prawego dziecka.
//Lewe poddrzewo prawego dziecka punktu obrotu staje si� prawym poddrzewem punktu obrotu.
//A wska�nik na w�ze�, wok� kt�rego odbywa si� obr�t.
template < typename keyType >
void RBtree < keyType > ::leftRotate(RBnode* const pivot) {
    if (pivot != NIL) {
        RBnode* const prc{
              pivot->right
        };
            if (prc == NIL) {
                throw std::exception{
                  "B��D: Prawe dziecko pivota nie mo�e by� NIL."
                };
            }
        pivot->right = prc->left;
        if (prc->left != NIL) {
            prc->left->parent = pivot;
        }
        if (pivot->parent == NIL) {
            root = prc;
        }
        else if (pivot == pivot->parent->left) {
            pivot->parent->left = prc;
        }
        else //if (pivot == pivot->parent->right)
        {
            pivot->parent->right = prc;
        }
        prc->parent = pivot->parent;
        prc->left = pivot;
        pivot->parent = prc;
    }
}
template<typename keyType>
void RBtree<keyType>::rightRotate(RBnode* const pivot)
{
    if (pivot != NIL)
    {
        RBnode* plc{ pivot->left };
        if (plc == NIL)
        {
            throw std::exception{ "B��D: Prawe dziecko pivota nie mo�e by� NIL." };
        }
        pivot->left = plc->right;
        if (plc->right != NIL)
        {
            plc->right->parent = pivot;
        }
        if (pivot->parent == NIL)
        {
            root = plc;
        }
        else if (pivot == pivot->parent->left)
        {
            pivot->parent->left = plc;
        }
        else //if (pivot == pivot->parent->right)
        {
            pivot->parent->right = plc;
        }
        plc->parent = pivot->parent;
        plc->right = pivot;
        pivot->parent = plc;
    }
}
//Przywraca w�a�ciwo�ci drzew czerwono-czarnych po wykonaniu wstawienia.
//A wska�nik do wstawianego w�z�a
template < typename keyType >
void RBtree < keyType > ::insertFixup(RBnode* insertedNode) {
    ++numRedNodes;
    while (insertedNode->parent->nodeColor == Color::RED) {
        if (insertedNode->parent == insertedNode->parent->parent->left) {
            RBnode* rightUncle = insertedNode->parent->parent->right;
            //Przypadek 1: Wujek wstawionego w�z�a jest czerwony
            //Przekoloruj rodzica i wujka w�z�a na czarno, a dziadka w�z�a na czerwono.
            //dalej w g�r� drzewa. Zmie� wska�nik wstawionego w�z�a, aby wskazywa� na jego poprzednika i sprawd�, czy nie dosz�o do naruszenia w�a�ciwo�ci
            if (rightUncle->nodeColor == Color::RED) {
                insertedNode->parent->nodeColor = Color::BLACK;
                rightUncle->nodeColor = Color::BLACK;
                insertedNode->parent->parent->nodeColor = Color::RED;
                insertedNode = insertedNode->parent->parent;
                ++numBlackNodes;
                --numRedNodes;
            }
            else {
                //Przypadek 2: Wujek wstawionego w�z�a jest czarny, a wstawiony w�ze� jest prawym dzieckiem
                //Wska� rodzica wstawionego w�z�a i wykonaj obr�t w lewo z rodzicem wstawionego w�z�a jako punktem obrotu.
                if (insertedNode == insertedNode->parent->right) {
                    insertedNode = insertedNode->parent;
                    leftRotate(insertedNode);
                }

                //Przypadek 3: Wujek wstawionego w�z�a jest czarny, a wstawiony w�ze� jest lewym dzieckiem
                //Pokoloruj rodzica wstawionego w�z�a na czarno, a dziadka na czerwono, a nast�pnie obr�� w prawo z
                //dziadkiem jako punktem obrotu. 
                insertedNode->parent->nodeColor = Color::BLACK;
                insertedNode->parent->parent->nodeColor = Color::RED;
                rightRotate(insertedNode->parent->parent);
            }
        }
        else //if (insertedNode->parent == insertedNode->parent->parent->right)
        {
            RBnode* leftUncle = insertedNode->parent->parent->left;

            //przyp1
            if (leftUncle->nodeColor == Color::RED) {
                insertedNode->parent->nodeColor = Color::BLACK;
                leftUncle->nodeColor = Color::BLACK;
                insertedNode->parent->parent->nodeColor = Color::RED;
                insertedNode = insertedNode->parent->parent;
                ++numBlackNodes;
                --numRedNodes;
            }
            else {
                //przyp 2
                if (insertedNode == insertedNode->parent->left) {
                    insertedNode = insertedNode->parent;
                    rightRotate(insertedNode);
                }

                //przyp3
                insertedNode->parent->nodeColor = Color::BLACK;
                insertedNode->parent->parent->nodeColor = Color::RED;
                leftRotate(insertedNode->parent->parent);
            }
        }
    }

    //Je�li w tym momencie kolor korzenia jest czerwony, zmieniamy kolor na czarny
    if (root->nodeColor == Color::RED) {
        ++numBlackNodes;
        --numRedNodes;
    }
    root->nodeColor = Color::BLACK;
}

template < typename keyType >
void RBtree < keyType > ::deleteFixup(RBnode* x) {
    RBnode* w;
    while (x != root && x->nodeColor == Color::BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->nodeColor == Color::RED) {
                w->nodeColor = Color::BLACK;
                x->parent->nodeColor = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->nodeColor == Color::BLACK && w->right->nodeColor == Color::BLACK) {
                w->nodeColor = Color::RED;
                x = x->parent;
                ++numRedNodes;
                --numBlackNodes;
            }
            else {
                if (w->right->nodeColor == Color::BLACK) {
                    w->left->nodeColor = Color::BLACK;
                    w->nodeColor = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->nodeColor = x->parent->nodeColor;
                x->parent->nodeColor = Color::BLACK;
                w->right->nodeColor = Color::BLACK;
                leftRotate(x->parent);
                x = root;
                --numRedNodes;
                ++numBlackNodes;
            }
        }
        else {
            w = x->parent->left;
            if (w->nodeColor == Color::RED) {
                w->nodeColor = Color::BLACK;
                x->parent->nodeColor = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->nodeColor == Color::BLACK && w->left->nodeColor == Color::BLACK) {
                w->nodeColor = Color::RED;
                x = x->parent;
                ++numRedNodes;
                --numBlackNodes;
            }
            else {
                if (w->left->nodeColor == Color::BLACK) {
                    w->right->nodeColor = Color::BLACK;
                    w->nodeColor = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->nodeColor = x->parent->nodeColor;
                x->parent->nodeColor = Color::BLACK;
                w->left->nodeColor = Color::BLACK;
                rightRotate(x->parent);
                x = root;
                ++numBlackNodes;
                --numRedNodes;
            }
        }
    }
    if (x->nodeColor != Color::BLACK) {
        --numRedNodes;
        ++numBlackNodes;
    }

    x->nodeColor = Color::BLACK;
}
/*
template<typename keyType>
void RBtree < keyType >::PreOrder(RBnode* const) {
    if (node != NIL) {
        std::cout<< node->key << " ";
        PreOrder(node->left);
        PreOrder(node->right);
    }
}

template<typename keyType>
void RBtree < keyType >::InOrder(RBnode* const) {
    if (node != NIL) {
        InOrder(node->left);
        std::cout << node->key << " ";
        InOrder(node->right)

    }
}

*/

template < typename keyType >
void RBtree < keyType > ::RB_insert(RBnode* insertedNode) {
    RBnode* trailing = NIL;
    RBnode* traverse = root;
    while (traverse != NIL) {
        trailing = traverse;
        if (insertedNode->key < traverse->key) {
            traverse = traverse->left;
        }
        else {
            traverse = traverse->right;
        }
    }
    insertedNode->parent = trailing;
    if (trailing == NIL) {
        root = insertedNode;
    }
    else if (insertedNode->key < trailing->key) {
        trailing->left = insertedNode;
    }
    else {
        trailing->right = insertedNode;
    }
    insertFixup(insertedNode);
}

//Pami�� jest zwalniana w tej funkcji
template < typename keyType >
void RBtree < keyType > ::RB_delete(RBnode* nodeToDelete) {
    RBnode* y = nodeToDelete;
    RBnode* replacement;
    Color originalColor = nodeToDelete->nodeColor;

    if (nodeToDelete->left == NIL) {
        replacement = nodeToDelete->right;
        transplant(nodeToDelete, nodeToDelete->right);
    }
    else if (nodeToDelete->right == NIL) {
        replacement = nodeToDelete->left;
        transplant(nodeToDelete, nodeToDelete->left);
    }
    else {
        y = minimum(nodeToDelete->right);
        originalColor = y->nodeColor;
        replacement = y->right;

        if (y->parent == nodeToDelete) {
            replacement->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = nodeToDelete->right;
            y->right->parent = y;
        }

        transplant(nodeToDelete, y);
        y->left = nodeToDelete->left;
        y->left->parent = y;
        //W�ze� y zostanie pokolorowany kolorem w�z�a, kt�ry usuwamy.
        //Je�li kolory w�z��w s� r�ne, to y zmieni kolor, a liczniki zostan� dopasowane
        if (y->nodeColor != nodeToDelete->nodeColor) {
            //y zmienia kolor na k.wezla ktory usuwamy
            if (nodeToDelete->nodeColor == Color::BLACK) {
                ++numBlackNodes;
                --numRedNodes;
            }
            else {
                ++numRedNodes;
                --numBlackNodes;
            }
        }

        y->nodeColor = nodeToDelete->nodeColor;
    }
    if (nodeToDelete->nodeColor == Color::BLACK) {
        --numBlackNodes;
    }
    else {
        --numRedNodes;
    }
    delete nodeToDelete;
    if (originalColor == Color::BLACK) {
        deleteFixup(replacement);
    }
}

template < typename keyType >
int RBtree < keyType > ::maximum(const int a, const int b) const {
    return (a > b) ? a : b;
}

template < typename keyType >
int RBtree < keyType > ::subtreeHeight(const RBnode* const subtreeRoot) const {
    //If our subtree is empty, return height of -1
    if (subtreeRoot == NIL) {
        return -1;
    }
    //Otherwise, return the maximum of the height of the left and right subtrees of the current node
    else {
        return maximum(subtreeHeight(subtreeRoot->left), subtreeHeight(subtreeRoot->right)) + 1;
    }
}

// Przydziela nowe czerwono czarne drzewo poprzez skopiowanie struktury innego.
// Wska�nik na rodzica kopiowanego w�z�a.
//Wska�nik na kopiowany w�ze�.
//Pointer na w�ze� NIL w drzewie, z kt�rego jest kopiowany.
template < typename keyType >
void RBtree < keyType > ::copyTree(RBnode* copyTo_parent, RBnode* copyFrom, RBnode* copyFrom_NIL) {
    if (copyFrom == copyFrom_NIL) {
        return;
    }
    RBnode* copyTo{
      new RBnode
    };
    copyTo->key = copyFrom->key;
    copyTo->nodeColor = copyFrom->nodeColor;
    copyTo->parent = copyTo_parent;
    copyTo->left = NIL;
    copyTo->right = NIL;
    if (root == NIL) {
        root = copyTo;
    }
    if (copyTo->key < copyTo_parent->key) {
        copyTo_parent->left = copyTo;
    }
    else {
        copyTo_parent->right = copyTo;
    }
    copyTree(copyTo, copyFrom->left, copyFrom_NIL);
    copyTree(copyTo, copyFrom->right, copyFrom_NIL);
}

template < typename keyType >
void RBtree < keyType > ::ASC(const RBnode* const node) const {
    if (node == NIL)
        return;
    ASC(node->left);
    std::cout << node->key << std::endl;
    ASC(node->right);
}

template < typename keyType >
void RBtree < keyType > ::DES(const RBnode* const node) const {
    if (node == NIL)
        return;
    DES(node->right);
    std::cout << node->key << std::endl;
    DES(node->left);
}
// Konstruktor dla RBtree. Przydziela w�ze� NIL i wskazuje korze� na NIL. W�ze� NIL jest pokolorowany na czarno.
template < typename keyType >
RBtree < keyType > ::RBtree() : NIL{
  new RBnode
}, numBlackNodes{
  0
}, numRedNodes{
  0
} {
    NIL->nodeColor = Color::BLACK;
    root = NIL;
}

// Konstruktor kopiuj�cy drzewo czerwono-czarne. Ustawia puste drzewo, a nast�pnie kopiuje dane z parametru drzewo.
template < typename keyType >
RBtree < keyType > ::RBtree(const RBtree& right) :
    NIL{
      new RBnode
}, numBlackNodes{
  right.numBlackNodes
}, numRedNodes{
  right.numRedNodes
} {
    NIL->nodeColor = Color::BLACK;
    root = NIL;
    copyTree(root, right.root, right.NIL);
}

// W tej funkcji nast�puje deallokacja pami�ci.
//destruktor drzewa czerwono-czarnego. Niszczy drzewo i zwalnia zaalokowan� pami��. Pami�� przydzielona do w�z�a NIL jest
//dealokowana w destruktorze.
template < typename keyType >
RBtree < keyType > ::~RBtree() {
    destroyTree();
    delete NIL;
}

//Pami�� jest alokowana w tej funkcji
//Wstawia do drzewa czerwono-czarnego w�ze� o okre�lonej warto�ci klucza.
template < typename keyType >
void RBtree < keyType > ::add(const keyType x) {
    //Alokacja nowego w�z�a, inicjalizacja go danymi przekazanymi do funkcji
    RBnode* newNode = new RBnode;
    newNode->parent = NIL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->nodeColor = Color::RED;
    newNode->key = x;
    RB_insert(newNode);
}

//Okre�la, czy podana warto�� klucza znajduje si� w drzewie. Warto�� wska�nika zwr�conego z wyszukiwania
//jest por�wnywana z NIL. Je�eli wska�nik nie jest NIL, to element zosta� odnaleziony i wyra�enie ocenia si� na true. 
//Je�eli wska�nik jest NIL, to wyszukiwanie nie powiod�o si� i wyra�enie jest interpretowane jako false.
//Warto�� kluczowa, kt�ra jest szukana w drzewie
//zwraca True, je�eli przekazana warto�� klucza znajduje si� w drzewie, w przeciwnym razie false
template < typename keyType >
bool RBtree < keyType > ::HasKey(const keyType keyValue) const {
    return (search(root, keyValue) != NIL);
}
template < typename keyType >
bool RBtree < keyType > ::isEmpty() const {
    return root == NIL;
}

//zwraca -1 kiedy puste drzewo
template < typename keyType >
int RBtree < keyType > ::Height() const {
    return subtreeHeight(root);
}

template < typename keyType >
void RBtree < keyType > ::destroyTree() {
    while (root != NIL) {
        RB_delete(root);
    }
}

template < typename keyType >
void RBtree < keyType > ::displayTree(const Order ord) const {
    RBnode* node = root;
    if (node == NIL) {
        return;
    }
    // Je�li warto�� x jest 0, umie�� drzewo w porz�dku rosn�cym.
    else if (ord == Order::ASC) {
        ASC(node);
    }
    // Je�li warto�� x wynosi 1, umie�� drzewo w porz�dku malej�cym.
    else if (ord == Order::DES) {
        DES(node);
    }
    else {
        std::cout << "Order nie w obrebie klasy enumeratora." << std::endl;
    }
}
//Przypisuje jedno drzewo do drugiego.Dok�adna struktura drzewa czerwono - czarnego jest kopiowana.
//Drzewo po prawej stronie instrukcji przypisania. (leftTree = rightTree)
//zwraca Odwo�anie do drzewa, kt�re zosta�o przypisane.
template < typename keyType >
RBtree < keyType >& RBtree < keyType > ::operator = (const RBtree < keyType >& right) {
    if (this != &right) {
        this->destroyTree();
        numBlackNodes = right.numBlackNodes;
        numRedNodes = right.numRedNodes;
        copyTree(root, right.root, right.NIL);
    }
    return *this;
}
template < typename keyType >
RBtree < keyType >& RBtree < keyType > ::operator += (const RBtree < keyType >& right) {
    return *this = (*this + right);
}

