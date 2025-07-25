## Example GPU

Consider the following example GPU.


![](../../../docs/img/gpu_as_cus_sms_eus.png)

![](../../../docs/img/cu_sm_eu.png)


Think about the following questions and try to come up with an answer
to them.

Consult the slides for help.

In the following, by "saturate the entire ..." we mean there's at least
a single unit of work for every piece of hardware.

For example, "to saturate an entire SIMD unit, four lanes wide" we need at least
four pieces of data.

----------------------------------------------------------------------

Base assumptions:
- the warp size is 32

While answering these, consider the example GPU and SM/CU given at the beginning.


### Grids, blocks, warps, threads

1. How many grids does it take, at least, to saturate the entire GPU?
    - What hardware component does a grid of blocks map to?
    - How many of them are there?
- 1 GRID -> GPU


2. How many blocks does it take, at least, to saturate the entire GPU?
    - What hardware component does a block of threads map to?
    - How many of them are there?
- block -> CU
- 8

3. What is the minimum amount of threads in a block to saturate an entire CU/SM?
    - What is the warp size?
    - What hardware component does a warp map to?
    - How many of these components are there in a single CU/SM?
- warp -> vector unit
- 128

4. How many threads does it take, in total, to saturate the entire GPU?
    - How many threads to sature a CU/SM (See question 3)?
    - How many blocks to saturate the entire GPU (see question 2)?
- 128*8 =n1024

### Miscellaneous

1. Can you, the user, control the number of threads in a warp (given a particular GPU)?
   - No
2. Can you, the user, control the number of warps in a block?
   - Yes
3. Can you, the user, control the number of threads in a block?
   - Yes
4. If you answered 'No' to question 2, consider it again after answering question 3.
