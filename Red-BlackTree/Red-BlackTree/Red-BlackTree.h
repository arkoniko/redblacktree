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
        keyType key; //Dane zawarte w wêŸle
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

    //Przeci¹¿one operatory
    RBtree < keyType >& operator = (const RBtree < keyType >&);
    RBtree < keyType > operator + (const RBtree < keyType >&) const;
    RBtree < keyType >& operator += (const RBtree < keyType >&);
    bool operator == (const RBtree&) const;
    bool operator != (const RBtree&) const;
};

//Zastêpuje jedno drzewo podrzêdne jako dziecko swojego rodzica innym drzewem podrzêdnym.
//Zastêpuje drzewo podrzêdne zakorzenione w wêŸle u drzewem podrzêdnym zakorzenionym w wêŸle v
//wskaznik na wezel do wymiany
//wskaznik na wezel zast. v w drzewie
template < typename keyType >
void RBtree < keyType > ::transplant(RBnode* const u, RBnode* const v) {
    //Sprawdza, czy chcemy przeszczepiæ korzeñ, lewe dziecko czy prawe dziecko
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

//Szuka drzewa dla okreœlonej wartoœci klucza. Zwraca NIL, jeœli wartoœæ nie zosta³a znaleziona.
//A wskaŸnik u¿ywany do wêz³a w drzewie. WskaŸnik ten przemierza przeszukiwane drzewo.
//Wartoœæ szukana w drzewie.
//A wskaŸnik do wêz³a zawieraj¹cego okreœlon¹ wartoœæ klucza.
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

//UWAGA: Jeœli prawe dziecko osi obrotu jest NIL, zostanie zg³oszony wyj¹tek.
//Wykonuje obrót w lewo wokó³ wêz³a obrotowego. Zak³ada, ¿e prawe dziecko punktu obrotu nie jest NIL.
//Zmieniaj¹ siê tylko wskaŸniki do wêz³ów, nie ma to wp³ywu na inne atrybuty.
//Prawe dziecko wêz³a obrotowego przyjmuje jego pozycjê w drzewie. Pivot staje siê lewym dzieckiem swojego prawego dziecka.
//Lewe poddrzewo prawego dziecka punktu obrotu staje siê prawym poddrzewem punktu obrotu.
//A wskaŸnik na wêze³, wokó³ którego odbywa siê obrót.
template < typename keyType >
void RBtree < keyType > ::leftRotate(RBnode* const pivot) {
    if (pivot != NIL) {
        RBnode* const prc{
              pivot->right
        };
            if (prc == NIL) {
                throw std::exception{
                  "B£¥D: Prawe dziecko pivota nie mo¿e byæ NIL."
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
            throw std::exception{ "B£¥D: Prawe dziecko pivota nie mo¿e byæ NIL." };
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
//Przywraca w³aœciwoœci drzew czerwono-czarnych po wykonaniu wstawienia.
//A wskaŸnik do wstawianego wêz³a
template < typename keyType >
void RBtree < keyType > ::insertFixup(RBnode* insertedNode) {
    ++numRedNodes;
    while (insertedNode->parent->nodeColor == Color::RED) {
        if (insertedNode->parent == insertedNode->parent->parent->left) {
            RBnode* rightUncle = insertedNode->parent->parent->right;
            //Przypadek 1: Wujek wstawionego wêz³a jest czerwony
            //Przekoloruj rodzica i wujka wêz³a na czarno, a dziadka wêz³a na czerwono.
            //dalej w górê drzewa. Zmieñ wskaŸnik wstawionego wêz³a, aby wskazywa³ na jego poprzednika i sprawdŸ, czy nie dosz³o do naruszenia w³aœciwoœci
            if (rightUncle->nodeColor == Color::RED) {
                insertedNode->parent->nodeColor = Color::BLACK;
                rightUncle->nodeColor = Color::BLACK;
                insertedNode->parent->parent->nodeColor = Color::RED;
                insertedNode = insertedNode->parent->parent;
                ++numBlackNodes;
                --numRedNodes;
            }
            else {
                //Przypadek 2: Wujek wstawionego wêz³a jest czarny, a wstawiony wêze³ jest prawym dzieckiem
                //Wska¿ rodzica wstawionego wêz³a i wykonaj obrót w lewo z rodzicem wstawionego wêz³a jako punktem obrotu.
                if (insertedNode == insertedNode->parent->right) {
                    insertedNode = insertedNode->parent;
                    leftRotate(insertedNode);
                }

                //Przypadek 3: Wujek wstawionego wêz³a jest czarny, a wstawiony wêze³ jest lewym dzieckiem
                //Pokoloruj rodzica wstawionego wêz³a na czarno, a dziadka na czerwono, a nastêpnie obróæ w prawo z
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

    //Jeœli w tym momencie kolor korzenia jest czerwony, zmieniamy kolor na czarny
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

//Pamiêæ jest zwalniana w tej funkcji
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
        //Wêze³ y zostanie pokolorowany kolorem wêz³a, który usuwamy.
        //Jeœli kolory wêz³ów s¹ ró¿ne, to y zmieni kolor, a liczniki zostan¹ dopasowane
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
// WskaŸnik na rodzica kopiowanego wêz³a.
//WskaŸnik na kopiowany wêze³.
//Pointer na wêze³ NIL w drzewie, z którego jest kopiowany.
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
// Konstruktor dla RBtree. Przydziela wêze³ NIL i wskazuje korzeñ na NIL. Wêze³ NIL jest pokolorowany na czarno.
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

// Konstruktor kopiuj¹cy drzewo czerwono-czarne. Ustawia puste drzewo, a nastêpnie kopiuje dane z parametru drzewo.
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

// W tej funkcji nastêpuje deallokacja pamiêci.
//destruktor drzewa czerwono-czarnego. Niszczy drzewo i zwalnia zaalokowan¹ pamiêæ. Pamiêæ przydzielona do wêz³a NIL jest
//dealokowana w destruktorze.
template < typename keyType >
RBtree < keyType > ::~RBtree() {
    destroyTree();
    delete NIL;
}

//Pamiêæ jest alokowana w tej funkcji
//Wstawia do drzewa czerwono-czarnego wêze³ o okreœlonej wartoœci klucza.
template < typename keyType >
void RBtree < keyType > ::add(const keyType x) {
    //Alokacja nowego wêz³a, inicjalizacja go danymi przekazanymi do funkcji
    RBnode* newNode = new RBnode;
    newNode->parent = NIL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->nodeColor = Color::RED;
    newNode->key = x;
    RB_insert(newNode);
}

//Okreœla, czy podana wartoœæ klucza znajduje siê w drzewie. Wartoœæ wskaŸnika zwróconego z wyszukiwania
//jest porównywana z NIL. Je¿eli wskaŸnik nie jest NIL, to element zosta³ odnaleziony i wyra¿enie ocenia siê na true. 
//Je¿eli wskaŸnik jest NIL, to wyszukiwanie nie powiod³o siê i wyra¿enie jest interpretowane jako false.
//Wartoœæ kluczowa, która jest szukana w drzewie
//zwraca True, je¿eli przekazana wartoœæ klucza znajduje siê w drzewie, w przeciwnym razie false
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
    // Jeœli wartoœæ x jest 0, umieœæ drzewo w porz¹dku rosn¹cym.
    else if (ord == Order::ASC) {
        ASC(node);
    }
    // Jeœli wartoœæ x wynosi 1, umieœæ drzewo w porz¹dku malej¹cym.
    else if (ord == Order::DES) {
        DES(node);
    }
    else {
        std::cout << "Order nie w obrebie klasy enumeratora." << std::endl;
    }
}
//Przypisuje jedno drzewo do drugiego.Dok³adna struktura drzewa czerwono - czarnego jest kopiowana.
//Drzewo po prawej stronie instrukcji przypisania. (leftTree = rightTree)
//zwraca Odwo³anie do drzewa, które zosta³o przypisane.
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

