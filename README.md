# Voxal
The Voxal Programming Language is a LISP-like functional programming language written in C++. It serves as my 2nd term OOP project.

## Example
```
(define iterate begin end clb
	(if (< begin end)
		(+ (clb begin) (iterate (+ 1 begin) end clb))))

(define fibonacci n
	(if (< n 2)
		n
		(+ (fibonacci (- n 1)) (fibonacci (- n 2)))))

(print (fibonacci 5))

(iterate 0 10
	(lambda x 
		(print x)))
```
