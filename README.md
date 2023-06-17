# Voxal
The Voxal Programming Language is a LISP-like functional programming language written in C++. It serves as my 2nd term OOP project.

## Example
```
(define while cond clb
	(clb)
	(if (cond) 
		(while cond clb)))

(define main a1 a2 a3
	(define s
		(if (< a1 a2)
			a1
			a2))

	(print s "\nHello, world")

	(while (< s 10)
		(set s (+ s 1))
		(print s)))
```
