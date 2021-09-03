#ifndef STRUCTS_H
#define STRUCTS_H

struct Parameter {
    bool isEnabled = true;
    QString fileName;
    unsigned long long int id;
};

typedef Parameter Parameter;

#endif // STRUCTS_H
