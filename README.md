
<div align="center">

# mutest

![CodeCov](https://img.shields.io/codecov/c/github/cute-engineewing/mutest?style=flat-square)

</div>

mutest: a little test framework written in C99 ! 

little sample:

```c
#include "mutest.h"

mutest_group(buffer_test)

mutest_test(buffer_creation)
{
    struct buffer buff = create_buffer(10);

    mutest_expect(buff.size == 10);
    mutest_expect_not_null(buff.data);

	return MUTEST_SUCCESS;
}

mutest_test(buffer_deletion)
{

    struct buffer buff = create_buffer(10);

    delete_buffer(&buff);

    mutest_expect(buff.size == 0);
    mutest_expect_null(buff.data);

	return MUTEST_SUCCESS;
}
```