(define createStudent
  (lambda (lastName firstName id)
	(list lastName firstName id)))

(define getLastName (lambda student (car student)))
(define getFirstName (lambda student (car (cdr student))))
(define getId (lambda student (car (cdr (cdr student)))))

(define compIdAndStudent
  (lambda (id student)
    (> id (getId student))))

(define compStudentsById
  (lambda (student1 student2)
    (compIdAndStudent (getId student1) student2)))

(define idMatch?
  (lambda (id student)
    (= id (getId student))))

(define studentData
  (list
    (createStudent (quote DeSmet) (quote Rafael) 20131145)
    (createStudent (quote Pauwels) (quote Koen) 20103548)
    (createStudent (quote Adhikari) (quote Ajay) 20136247)
    (createStudent (quote Deruyttere) (quote Thierry) 20130626)
    (createStudent (quote Rys) (quote Arkadiusz) 20134579)
    (createStudent (quote Bellemans) (quote Olivier) 20135536)
))

(define koen (createStudent (quote Pauwels) (quote Koen) 20103548))
(define ajay (createStudent (quote Adhikari) (quote Ajay) 20136247))

(define insertStudent (insert compStudentsById))
(define retrieveStudent (retrieve compIdAndStudent idMatch?))
(define deleteStudent (delete compIdAndStudent idMatch?))
