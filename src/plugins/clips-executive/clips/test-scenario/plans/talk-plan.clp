
(defrule plan-talk-expand
	?g <- (goal (mode SELECTED) (id ?goal-id) (class TALK))
	=>
	(bind ?plan-id (sym-cat ?goal-id -PLAN))
	(assert
		(plan (id ?plan-id) (goal-id ?goal-id) (type SEQUENTIAL))
		(plan-action (id 10) (goal-id ?goal-id) (plan-id ?plan-id)
                 (duration 4.0)
		             (action-name say-hello)
		             (param-names name) (param-values peggy))
		;(plan-action (id 11) (goal-id ?goal-id) (plan-id ?plan-id)
		;             (action-name lock)
		;             (param-names name)
		;	            (param-values lock1))
		(plan-action (id 20) (goal-id ?goal-id) (plan-id ?plan-id)
		             (duration 4.0)
		             (action-name print)
								 (param-names severity text)
		             (param-values warn "This is a print test"))
		;(plan-action (id 21) (goal-id ?goal-id) (plan-id ?plan-id)
		;             (action-name unlock)
		;						  (param-names name)
		;             (param-values lock1))
		;(plan-action (id 22) (goal-id ?goal-id) (plan-id ?plan-id)
		;             (action-name lock)
		;						  (param-names name)
		;             (param-values lock1))
		;(plan-action (id 23) (goal-id ?goal-id) (plan-id ?plan-id)
		;             (action-name unlock)
		;             (param-names name)
		;             (param-values lock1))
		(plan-action (id 30) (goal-id ?goal-id) (plan-id ?plan-id)
		             (duration 4.0)
		             (action-name say-goodbye) (param-values peggy))
		(plan-action (id 40) (goal-id ?goal-id) (plan-id ?plan-id)
		             (duration 4.0)
		             (action-name say-hello-again)
		             (param-names name) (param-values peggy))
		(plan-action (id 50) (goal-id ?goal-id) (plan-id ?plan-id)
		             (duration 4.0)
		             (action-name say-cleanup))
	)
)
