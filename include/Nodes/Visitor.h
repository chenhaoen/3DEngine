#pragma once

class Node;
class Group;
class Geode;
class Geometry;

class Visitor
{

public:
    Visitor();
    virtual ~Visitor();

    virtual void apply(Node &);
    virtual void apply(Group &);
    virtual void apply(Geode &);
    virtual void apply(Geometry &);

private:
};
