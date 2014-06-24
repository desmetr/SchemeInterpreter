(define empty?
  (lambda l
    (= 0 (length l))))

(define range
  (lambda (bgn end)
    (if (= bgn end)
      (list)
      (cons bgn (range (+ bgn 1) end)))))

(define fib
  (lambda n
    (if (< n 2)
      1
      (+ (fib (- n 1)) (fib (- n 2))))))

(define fold
  (lambda (f init l)
    (if (empty? l)
      init
      (fold f (f (car l) init) (cdr l)))))

(define filter
  (lambda (condition l)
    (if (empty? l)
      (list)
      (let (rest (filter condition (cdr l)))
        (if (condition (car l))
          (cons (car l) rest)
          rest)))))

(define map
  (lambda (trans l)
    (if (empty? l)
      (list)
      (cons (trans (car l)) (map trans (cdr l))))))
