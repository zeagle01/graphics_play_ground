# constraints

- general constraint
    - given one stencil
        - $x=(x_0,x_1,\cdots,x_m)^T$
            - m=2 for spring
            - m=3 for triangle
            - m=4 for bending ,ee vt collision etc.
            - $\cdots$
    - constraint fucntion 
        - $wx=0$
            - $w=(w_0,w_1,\cdots,w_m)^T$
                - $w=(-1,1)^T$ for spring
                - $\cdots$

    - we want to build local element wise equation $Ax=b$,
         so that the position is equal in constraint space,one way to do it is to project position to constraint space
        - the projection normal equation  

          - $ww^Tx=ww^Tx^p$

        - so for each edge

          - $A=ww^T$
          - $b=ww^Tx^p$  or $b=wd^p$

    - use jacobi iteration solver to solve
        - for each constraint 
            - $\bar{A}x=b-\hat{A}x$
                - or $\bar{A}x=wd^p-\hat{A}x$
                - where $\bar{A},\hat{A}$ is the diagnal ,offdiagnal part of $A$ respectively 

                - take spring consraint for example:

                    - $A=ww^T=\begin{pmatrix}1&-1\\-1&1 \end{pmatrix}$

                    - $\bar{A}=\begin{pmatrix}1&0\\0&1 \end{pmatrix}$

                    - $\hat{A}=\begin{pmatrix}0&-1\\-1&0 \end{pmatrix}$

                    - $x_0=-d^p+x_1$

                    - $x_1=d^p+x_0$

    - routine for every constraint 
        - compute $d^p$ or $x^p$
        - using $w$,$d^p$ compute $A,b$
            




- spring stretch
    - given one spring edge
        - $x=(x_0,x_1)$
        - $w=(-1,1)$
    - $d^p=l^0d/|d|$

