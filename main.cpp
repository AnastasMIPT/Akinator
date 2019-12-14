#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstdarg>
#include <AnastasLib\Stack.h>

const int DataSize = 100;
const int Question = 1;
const int Object = 2;
const int LenMassage = 150;
const int LenStringTree = 500;
const int LenStrPath = 100;
struct Node
{
    char* data;
    Node* left;
    Node* right;
    int type;
};

Node* CreateNode (char* data, int type);
Node* DelNode (Node* node);
Node* AkinatorInit (char* Property, char* LeftObj, char*RightObj);
Node* FindObj (Node* root);
int Say (Node* node);
void AddNewObj (Node* root, char* Obj, char* Difference);
int answer ();
void AskAndAdd (Node* node);
void PrintNods (Node* node, FILE* f_out);
void NodePrint (Node* node, FILE* f_out);
void TreePrint (Node* root, FILE* f_out);
void SaveTree (Node* root, FILE* f_sav);
void Fgets (char* str, int symb, FILE* f_in);
void DeleteTree (Node* root);
Node* GetTreeFromFile (Node* root, FILE* f_in);
void SayWithSound (char* format, ...);
char* dfs (Node* root, int depth, char* str);
char* PathToObj (char* Obj, Node* root);
char* PropertiesOfObj (char* Obj, Node* root);
void Differents (char* Obj1, char* Obj2, Node* root);
int main () {

    FILE* f_in  = fopen ("tree.txt", "r");
    //Node* root = AkinatorInit ("animal", "Lun", "bear");
    Node* root;
    root = GetTreeFromFile (root, f_in);

    Differents ("Mendeleev", "Vladimir Putin", root);

    char* str = PropertiesOfObj ("a whale", root);
    printf ("%s\n", str);
    SayWithSound ("espeak -ven -s130 \"%s\"", str);

    SayWithSound ("espeak -ven -s130 \"Helloo guys\"");

    int n = 1;
    while (n) {
        if (!FindObj(root)) {
            printf("ERROR empty leaf!\n");
        }
        printf ("Repeat?\n");
        SayWithSound ("espeak -ven -s130 \"Repeat?\"");
        printf ("0. NO  1. YES\n");
        fflush(stdin);
        scanf ("%d", &n);
    }


    printf ("Save tree?\n");
    SayWithSound ("espeak -ven -s130 \"Save tree?\"");
    printf ("0. NO  1. YES\n");
    scanf ("%d",&n);
    if (n) {
        FILE* f_sav = fopen ("tree.txt", "w");
        SaveTree (root, f_sav);
    }

    printf ("Draw tree?\n");
    SayWithSound ("espeak -ven -s130 \"Draw tree?\"");
    printf ("0. NO  1. YES\n");
    scanf ("%d",&n);
    if (n) {
        FILE* f_out = fopen ("F:\\Graphs\\output.dot", "w");
        TreePrint (root, f_out);
    }


    DeleteTree(root);

    return 0;
}

/*! Функция, выводящая различия между Obj1 и Obj2
*
*	@param [in] Obj1 данные первого объекта
*	@param [in] Obj2 данные второго объекта
*	@param [in] root корень дерева
*
*	@return возвращает строку, различий между объектами
*
*/

void Differents (char* Obj1, char* Obj2, Node* root) {
    char * Prop1 = PropertiesOfObj (Obj1, root);
    char * Prop2 = PropertiesOfObj (Obj2, root);
    char* ptr1 = Prop1;
    char* ptr2 = Prop2;
    while (*ptr1 == *ptr2) {
        ptr1++;
        ptr2++;
    }
    if (*ptr1 != '\0') {
        char* res1 = (char*) calloc(DataSize, sizeof(char));
        res1 = strcat(res1, Obj1);
        res1 = strcat(res1, " ");
        res1 = strcat(res1, ptr1);
        printf ("%s\n", res1);
        SayWithSound ("espeak -ven -s130 \"%s\"", res1);
        free (res1);
    }
    if (*ptr2 != '\0') {
        char* res2 = (char*) calloc(DataSize, sizeof(char));
        res2 = strcat(res2, Obj2);
        res2 = strcat(res2, " ");
        res2 = strcat(res2, ptr2);
        printf("%s\n", res2);
        SayWithSound ("espeak -ven -s130 \"%s\"1", res2);
        free(res2);
    }
    free (Prop1);
    free (Prop2);

}
char* PropertiesOfObj (char* Obj, Node* root) {
    char* path = PathToObj (Obj, root);
    char* result = (char*) calloc (LenStrPath, sizeof (char));
    Node* ptr = root;
    char* lec = strtok (path,"*");
    while (lec != NULL && !(ptr->right->type == Object && ptr->left->type == Object)) {
        lec = strtok (NULL, "*");
        if (lec == NULL) break;
        if (strcmp (ptr->right->data, lec) == 0)
        {
            result = strcat (result, ptr->data);
            result = strcat (result, " ");
            ptr = ptr->right;
        }
        else
        {
            ptr = ptr->left;
        }
    }
    if (strcmp (ptr->right->data, Obj) == 0)
    {
        result = strcat (result, ptr->data);
    }
    free (path);
    return result;
}

/*! Функция, возвращающая путь до узла
*
*	@param [in] Obj данные объекта, к которому необходимо найти путь
*	@param [in] root корень дерева
*
*	@return возвращает строку, содержащую путь до объекта
*
*/
char* PathToObj (char* Obj, Node* root) {
    char* string = (char*) calloc (LenStringTree, sizeof (char));
    dfs (root, 0 , string);
    free(malloc(100));
    char* ptr = strstr (string, Obj);

    if (ptr == nullptr) {
        printf ("ERROR This object isn't in the tree.\n");
        return nullptr;
    }

    int depth = *(ptr - 2) - 1;
    char** path = (char**) calloc (LenStrPath, sizeof (char*));
    int SizePath = 0;

    while (depth != '0') {
        char* data = (char*) calloc (DataSize, sizeof (char));
        while (*ptr != depth) ptr++;
        ptr += 2;
        int n = 0;
        sscanf (ptr, "%[^0-9] %n", data, &n);
        ptr += n;
        *(data + n - 1) = '\0';
        *path = data;
        path++;
        SizePath++;
        depth--;
    }

    path--;
    char* Result = (char*) calloc (LenStrPath, sizeof (char));
    for (int i = 0; i < SizePath; i++, path--) {
        Result = strcat (Result, *path);
        Result = strcat (Result, "*");
    }

    path++;

    for (int i = 0; i < SizePath; i++, path++) {
        free (*path);
    }

    free (string);

    return Result;
}
char* dfs (Node* root, int depth, char* str) {

    if (root) {
        depth = depth + 1;
        dfs (root->left, depth, str);
        dfs (root->right, depth, str);
        sprintf (str, "%s%d ", str, depth);
        sprintf (str, "%s%s ", str, root->data);
    }
    return str;
}


void SayWithSound (char* format, ...) {
    va_list ptr;
    va_start (ptr, format);
    char* massage = (char*) calloc (LenMassage, sizeof (char));
    vsprintf (massage, format, ptr);
    va_end (ptr);
    system (massage);
}

Node* GetTreeFromFile (Node* root, FILE* f_in) {

    fscanf (f_in, "(");
    char* data = (char*) calloc (DataSize, sizeof (char));
    int n = 0;
    fscanf (f_in, "%[^()] %n", data, &n);
    if (n == 0)
        return nullptr;
    int type = Object;
    if (*data == '?') type = Question;
    if (type == Question) data = data + 1;
    root = CreateNode (data, type);
    root->left  = GetTreeFromFile (root->left, f_in);
    root->right = GetTreeFromFile (root->left, f_in);
    fscanf (f_in,")");
    return root;
}

void DeleteTree (Node* root) {

    if (root) {
        DeleteTree (root->left);
        DeleteTree (root->right);
        DelNode (root);
    }
}

void SaveTree (Node* root, FILE* f_sav) {
    if (root) {
        fprintf(f_sav, "(");
        if (root->type == Question) fprintf (f_sav, "?");
        fprintf(f_sav, "%s", root->data);
        SaveTree (root->left, f_sav);
        SaveTree (root->right, f_sav);
        fprintf(f_sav, ")");
    }
}

void TreePrint (Node* root, FILE* f_out) {
    assert(root);
    assert(f_out);


    fprintf (f_out, "digraph {\n");

    NodePrint(root, f_out);
    PrintNods (root, f_out);

    fprintf(f_out, "}\n");
    fclose(f_out);
}

void NodePrint (Node* node, FILE* f_out) {
    fprintf (f_out, "node%p [label=\"%s%c\", shape=box];\n", node, (char*) node->data,
             node->left && node->right ? '?' : ' ');
}


void PrintNods (Node* node, FILE* f_out) {
    assert (node);
    assert (f_out);

    if (node->left != nullptr && node->right != nullptr) {
        NodePrint(node->left, f_out);
        NodePrint(node->right, f_out);

        fprintf (f_out, "node%p -> node%p [color=\"red\", label=\"Нет\"];\n", node, node->left);
        fprintf (f_out, "node%p -> node%p [color=\"green\", label=\"Да\"];\n", node, node->right);

        PrintNods (node->right, f_out);
        PrintNods (node->left,  f_out);
    }

}

void Fgets (char* str, int symb, FILE* f_in) {
    int c;
    for (char* ptr = str; (c = getc (f_in)) != symb; ptr++) *ptr = c;
}

void AskAndAdd (Node* node) {

    char* Hallmark = (char*) calloc (DataSize, sizeof (char));
    char* Obj   = (char*) calloc (DataSize, sizeof (char));

    printf ("What did you have in mind?\n");
    SayWithSound ("espeak -ven -s130 \"What did you have in mind?\"");
    fflush (stdin);
    Fgets (Obj, '\n', stdin);

    printf ("What's the hallmark of this?\n");
    SayWithSound ("espeak -ven -s130 \"What's the hallmark of this?\"");
    Fgets (Hallmark, '\n', stdin);

    AddNewObj (node, Obj, Hallmark);

    free (Hallmark);
    free (Obj);
}

void AddNewObj (Node* root, char* Obj, char* Difference) {
    root->left  = CreateNode (root->data, Object);
    root->right = CreateNode (Obj, Object);
    strcpy (root->data, Difference);
    root->type = Question;
}

Node* FindObj (Node* root) {

    if (root == nullptr) {
        return nullptr;
    }


    if (root->type == Question) {

        bool answer = Say (root);
        if (answer)
            return FindObj (root->right);
        else
            return FindObj (root->left);

    }
    else
    {

        bool answer = Say (root);
        if (answer)
            return root;
        else
            AskAndAdd (root);
    }
}

int Say (Node* node) {
    if (node->type == Question) {
        printf ("Is this %s?\n", node->data);
        SayWithSound ("espeak -ven -s130 \"Is this %s?\"", node->data);
    }
    else {
        printf("I think this is %s.\n", node->data);
        SayWithSound ("espeak -ven -s130 \"I think this is %s.\"", node->data);
    }
    return (bool) answer();
}

int answer () {
    int ans = -1;
    printf ("0. NO  1. YES\n");
    fflush (stdin);
    scanf ("%d", &ans);
    if (ans != 0 && ans != 1) {
        printf ("Press 1, if YES. 0 - if NO.\n");
        answer();
    }
    return ans;
}

Node* AkinatorInit (char* Property, char* LeftObj, char*RightObj) {

    Node* root  = CreateNode (Property, Question);
    root->left  = CreateNode (LeftObj,  Object);
    root->right = CreateNode (RightObj, Object);

    return root;
}

Node* CreateNode (char* data, int type) {
    Node* node  = (Node*) calloc (1, sizeof (Node));
    node->data  = (char*) calloc (DataSize, sizeof (char));
    strcpy (node->data, data);
    node->left  = nullptr;
    node->right = nullptr;
    node->type  = type;

    return node;
}

Node* DelNode (Node* node) {
    free (node->data);
    free (node->left);
    free (node->right);
}