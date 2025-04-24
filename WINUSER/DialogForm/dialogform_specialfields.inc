macro specialfields_define_dialogform name{

	struc event procaddr\{
		name#@EventStack equ .
		. dptr procaddr
	\} 

	struc notify procaddr\{
		name#@NotifyStack equ .
		. dptr procaddr
	\} 

	struc hook procaddr\{
		name#@HookStack equ .
		. dptr procaddr
	\} 
}

macro secialfields_inherit_dialogform parent, name{
	irpv ParentEvent, parent#@EventStack\{
		name#@EventStack equ ParentEvent
	\}
	irpv ParentNotify, parent#@NotifyStack\{
		name#@NotifyStack equ ParentNotify
	\}
	irpv ParentHook, parent#@HookStack\{
		name#@HookStack equ ParentHook
	\}
}

macro specialfields_ends_dialogform name{
	name#.controls_offsets @const irpv cntrl, name#@ControlStack\{ dptr name#.\\#cntrl \}
	
	macro Make_events\{
		name#.events.counter = 0
		if name#.events.count <> 0
			dw name#.events.count
		end if
		irpv evnt, name#@EventStack\\{
			dw evnt, name#.\\#evnt
			name#.events.counter = name#.events.counter + 1
		\\}
		name#.events.count = name#.events.counter
	\}
	name#.events @const Make_events
	purge Make_events

	macro Make_notifications\{
		name#.notifications.counter = 0
		if name#.notifications.count <> 0
			dw name#.notifications.count
		end if
		irpv ntfy, name#@NotifyStack\\{
			dw ntfy, name#.\\#ntfy
			name#.notifications.counter = name#.notifications.counter + 1
		\\}
		name#.notifications.count = name#.notifications.counter
	\}
	name#.notifications @const Make_notifications
	purge Make_notifications

	macro Make_hooks\{
		name#.hooks.counter = 0
		if name#.hooks.count <> 0
			dw name#.hooks.count
		end if
		irpv hook, name#@HookStack\\{
			dw hook, name#.\\#hook
			name#.hooks.counter = name#.hooks.counter + 1
		\\}
		name#.hooks.count = name#.hooks.counter
	\}
	name#.hooks @const Make_hooks
	purge Make_hooks
}

add_specialfields dialogform
