(define range (lambda (bgn end) 
  (if (< bgn end)
    (cons bgn (range (+ bgn 1) end)) 
    (list))))

(define empty?
  (lambda l
    (= 0 (length l))))

(define max2
  (lambda (a b)
    (if (> a b)
      a
      b)))

(define accumulate 
  (lambda (l f acc)
    (if (empty? l)
      acc
      (accumulate (cdr l) f (f (car l) acc)))))

(define square
  (lambda a
    (* a a)))

(define fib
  (lambda n
    (if (< n 2)
      1
      (+ (fib (- n 1)) (fib (- n 2))))))
