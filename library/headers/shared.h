#ifndef SHARED_H
#define SHARED_H

struct SparseDoubleLinkedMatrixElement {
    double value;
    SparseDoubleLinkedMatrixElement *nextLine, *nextColumn;
};

struct SparseDoubleLinkedMatrix {
    std::string name;
    std::vector<SparseDoubleLinkedMatrixElement*> linePointer, columnPointer;
};

constexpr char DLSMDelimiter = ';';


#endif //SHARED_H
