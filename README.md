
<div align="center">

# mutest

![CodeCov](https://img.shields.io/codecov/c/github/cute-engineewing/mutest?style=flat-square)

</div>

mutest: a little test framework written in C99 ! 

little sample:

```c
#include "mutest.h"

MUTEST_GROUP(buffer_test)

MUTEST_TEST(buffer_creation)
{
    struct buffer buff = create_buffer(10);

    muexpect(buff.size == 10);
    muexpect_not_null(buff.data);

	return MUTEST_SUCCESS;
}

mutest_test(buffer_deletion)
{

    struct buffer buff = create_buffer(10);

    delete_buffer(&buff);

    muexpect(buff.size == 0);
    muexpect_null(buff.data);

	return MUTEST_SUCCESS;
}
```