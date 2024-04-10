#ifndef SHARED_H
#define SHARED_H

struct SparseDoubleLinkedMatrixElement {
    double value;
    SparseDoubleLinkedMatrixElement *nextLine, *nextColumn;
};

class SparseDoubleLinkedMatrix {
public:
    std::vector<SparseDoubleLinkedMatrixElement*> linePointer, columnPointer;
};

constexpr char DLSMDelimiter = ';';


#endif //SHARED_H
