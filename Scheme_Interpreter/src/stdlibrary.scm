(define range (lambda (bgn end) (if (< bgn end) (cons bgn (range (+ bgn 1) end)) (list))))
