(define createStudent
  (lambda (lastName firstName id)
	(list lastName firstName id)))

(define getLastName (lambda student (car student)))
(define getFirstName (lambda student (car (cdr student))))
(define getId (lambda student (car (cdr (cdr student)))))

(define compById
  (lambda (student1 student2)
	(> (getId student1) (getId student2))))

(define idEqual?
  (lambda (student1 student2)
	(= (getId student1) (getId student2))))

(list
  (createStudent (quote Pauwels) (quote Koen) 20103548))
