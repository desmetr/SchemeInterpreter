
(define createNode 
  (lambda (item leftChild rightChild)
    (list item leftChild rightChild)))

(define getItem car)

(define getChild 
  (lambda (node which)
    (if (= which 0)
      (car (cdr node))
      (car (cdr (cdr node))))))

(define getExistingChildren
  (lambda node
    (filter
      (lambda child (not (empty? child)))
      (map
        (lambda which (getChild node which))
        (list 0 1)))))

(define connectNodes
  (lambda (parent child which)
    (if (= which 0)
      (createNode (getItem parent) child (getChild parent 1))
      (createNode (getItem parent) (getChild parent 0) child))))

(define insert
  (lambda comp 
    (lambda (item node)
      (if (empty? node)
        (createNode item (list) (list))
        (let (which (comp item (getItem node)))
          (connectNodes 
            node 
            ((insert comp) item (getChild node which))
            which))))))

(define retrieveNode
  (lambda (comp equals?)
    (lambda (item node)
      (if (empty? node) 
        node
        (if (equals? item (getItem node))
          node
          ((retrieveNode comp equals?) item (getChild node (comp item (getItem node)))))))))

(define retrieve
  (lambda (comp equals?)
    (lambda (item node)
      (getItem ((retrieveNode comp equals?) item node)))))

(define popInorderSuccessor
  (lambda node
    (let (popLeftmostNode 
           (lambda node2
             (if (empty? (getChild node2 0))
               (list (getChild node2 1) (getItem node2))
               (let (pairNewchildItem (popLeftmostNode (getChild node2 0)))
                 (list 
                   (createNode (getItem node2) (car pairNewchildItem) (getChild node2 1))
                   (car (cdr pairNewchildItem)))))))
      (popLeftmostNode (getChild node 1)))))

(define delete
  (lambda (comp equals?)
    (lambda (item node)
      (if (not (equals? item (getItem node)))
        (let (which (comp item (getItem node)))
          (connectNodes
            node
            ((delete comp equals?) item (getChild node which))
            which))
        (let (children (getExistingChildren node))
          (if (empty? children)
            (list)
            (if (= 1 (length children))
              (car children)
              (let (pairNewchildItem (popInorderSuccessor node))
                (createNode
                  (car (cdr pairNewchildItem))
                  (getChild node 0)
                  (car pairNewchildItem))))))))))
