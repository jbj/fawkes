
;---------------------------------------------------------------------------
;  protobuf.clp - protobuf message templates
;
;  Created: Fri Feb 08 15:42:52 2013
;  Copyright  2013  Tim Niemueller [www.niemueller.de]
;  Licensed under BSD license, cf. LICENSE file
;---------------------------------------------------------------------------

(deftemplate protobuf-msg
  (slot type (type STRING))
  (slot comp-id (type INTEGER))
  (slot msg-type (type INTEGER))
  (slot rcvd-via (type SYMBOL) (allowed-values STREAM BROADCAST))
  (multislot rcvd-from (cardinality 2 2))
  (slot client-type (type SYMBOL) (allowed-values SERVER CLIENT PEER))
  (slot client-id (type INTEGER))
  (slot ptr (type EXTERNAL-ADDRESS))
)

(deftemplate protobuf-receive-failed
  (slot comp-id (type INTEGER))
  (slot msg-type (type INTEGER))
  (slot rcvd-via (type SYMBOL) (allowed-values STREAM BROADCAST))
  (multislot rcvd-from (cardinality 2 2))
  (slot client-id (type INTEGER))
  (slot message (type STRING))
)

(deffunction pb-is-broadcast (?rcvd-via)
  (eq ?rcvd-via BROADCAST)
)

(defrule protobuf-cleanup-receive-failed
  (declare (salience -4000))
  ?f <- (protobuf-receive-failed (comp-id ?cid) (msg-type ?mt)
				 (rcvd-from ?host ?port) (message ?msg))
  =>
  (retract ?f)
  (printout t "Protobuf rcv fail for " ?cid ":" ?mt " from " ?host ":" ?port ": " ?msg crlf)
)

(defrule protobuf-cleanup-message
  (declare (salience -4000))
  ?pf <- (protobuf-msg)
  =>
  (retract ?pf)
)

(defrule protobuf-load-config
  (declare (salience -900))
  (init)
  =>
  (load-config "/clips-protobuf")
)

(defrule protobuf-silence-debug-facts
  (declare (salience -1000))
  (init)
  (confval (path "/clips-protobuf/unwatch-facts") (type STRING) (is-list TRUE) (list-value $?lv))
  =>
  (printout t "Disabling watching of the following facts: " ?lv crlf)
  (foreach ?v ?lv (unwatch facts (sym-cat ?v)))
)

(defrule silence-debug-rules
  (declare (salience -1000))
  (init)
  (confval (path "/clips-protobuf/unwatch-rules") (type STRING) (is-list TRUE) (list-value $?lv))
  =>
  (printout t "Disabling watching of the following rules: " ?lv crlf)
  (foreach ?v ?lv (unwatch rules (sym-cat ?v)))
)

; (defrule protobuf-client-connected
;   (declare (salience 1))
;   (protobuf-client-connected ?id ?host ?port)
;   =>
;   (if (debug 2) then
;     (printout t "Client " ?id " connected from " ?host ":" ?port crlf))
; )

; (defrule protobuf-client-disconnected
;   (declare (salience 1))
;   (protobuf-client-disconnected ?id)
;   =>
;   (if (debug 2) then (printout t "Client " ?id " disconnected" crlf))
; )

