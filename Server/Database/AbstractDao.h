//
//
//

#ifndef ABSTRACTDAO_H
#define ABSTRACTDAO_H

#include "DbPool.h"

template<typename Type>
class AbstractDao
{
public:
    AbstractDao() {
        mPool.initPool(5);
    }

    virtual std::optional<std::vector<Type>> getAll() = 0;
    virtual std::optional<Type> getById(int id) = 0;
    virtual bool update(Type data) = 0;
    virtual bool deleteById(int id) = 0;
    virtual bool insert(Type data) = 0;

protected:
    DbPool mPool;

};

#endif //ABSTRACTDAO_H
