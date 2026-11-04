match =DIALOGFORM_SPECIALFIELDS, DIALOGFORM_SPECIALFIELDS{
	include "FASM_OOP/WINUSER/DialogForm/dialogform_specialfields.inc"
}

macro specialfields_define_taskdialog name{
	struc tevent procaddr\{
		name#@TEventStack equ .
		. dptr procaddr
	\} 

	match =name#@task_btn_last_id, name#@task_btn_last_id\{
		name#@task_btn_last_id equ TASKDIALOG_FIRST_BUTTON_ID
	\}
	match =name#@task_radio_last_id, name#@task_radio_last_id\{
		name#@task_radio_last_id equ TASKDIALOG_FIRST_RADIO_ID
	\}

	macro @TPushButton bt_name, text=""\{
		name#@task_push_buttons equ bt_name, text, name#@task_btn_last_id
		name#.\#bt_name\#.id = name#@task_btn_last_id
		rept 1 _cnt : name#@task_btn_last_id + 1 \\{
			restore name#@task_btn_last_id
			name#@task_btn_last_id equ _cnt
		\\}
	\}

	macro @TRadioButton bt_name, text=""\{
		name#@task_radio_buttons equ bt_name, text, name#@task_radio_last_id
		name#.\#bt_name\#.id = name#@task_radio_last_id
		rept 1 _cnt:name#@task_radio_last_id + 1 \\{
			restore name#@task_radio_last_id
			name#@task_radio_last_id equ _cnt
		\\}
	\}

	macro @TTitle text\{
		\local ..text
		name#@task_title equ ..text @const cwstr text
		const TTITLE_PTR = ..text
	\}

	macro @TContent text\{
		\local ..text
		name#@task_content equ ..text @const cwstr text
		const TCONTENT_PTR = ..text
	\}

	macro @TCheckBox text\{
		\local ..text
		name#@task_checkbox equ ..text @const cwstr text
		const TCHECKBOX_PTR = ..text
	\}

	macro @TExpandedControlText text\{
		\local ..text
		name#@task_expanded_control equ ..text @const cwstr text
		const TEXPANDED_CONTROL_PTR = ..text
	\}

	macro @TExpandedText text\{
		\local ..text
		name#@task_expanded equ ..text @const cwstr text
		const TEXPANDED_PTR = ..text
	\}

	macro @THeader text\{
		\local ..text
		name#@task_header equ ..text @const cwstr text
		const THEADER_PTR = ..text
	\}

	macro @TCollapsedControlText text\{
		\local ..text
		name#@task_collapsed_control equ ..text @const cwstr text
		const TCOLLAPSED_CONTROL_PTR = ..text
	\}

	macro @TFooter text\{
		\local ..text
		name#@task_footer equ ..text @const cwstr text
		const TFOOTER_PTR = ..text
	\}
}

macro secialfields_inherit_taskdialog parent, name{
	irpv bt_record, parent#@task_push_buttons\{
		name#@task_push_buttons equ bt_record
		match bt_name=,text=,id, bt_record\\{
			name#.\\#bt_name\\#.id = id
		\\}
	\}
	match =parent#@task_btn_last_id, parent#@task_btn_last_id\{rept 0\{\}rept 1\{
		name#@task_btn_last_id equ parent#@task_btn_last_id
	\}

	irpv bt_record, parent#@task_radio_buttons\{
		name#@task_radio_buttons equ bt_record
		match bt_name=,text=,id, bt_record\\{
			name#.\\#bt_name\\#.id = id
		\\}
	\}
	match =parent#@task_radio_last_id, parent#@task_radio_last_id\{rept 0\{\}rept 1\{
		name#@task_radio_last_id equ parent#@task_radio_last_id
	\}
}

macro make@task_buttons [bt_args]{
	match id=, name, bt_args\{
		TASKDIALOG_BUTTON id, name
	\}
}

macro specialfields_ends_taskdialog name{
	purge @TPushButton, @TRadioButton
	irpv bt_record, name#@task_push_buttons\{
		\local ..constText, task_btn
		match bt_name=,text=,id, bt_record\\{
			..constText cwstr text
			task_btn equ id, ..constText
		\\}
		\common
		name#.pushBtnArray @const make@task_buttons task_btn
	rept 0\{\}rept 1\{
		name#.pushBtnArray = NULL
	\}
	name#.TPUSH_BTN_COUNT = name#@task_btn_last_id - TASKDIALOG_FIRST_BUTTON_ID

	irpv bt_record, name#@task_radio_buttons\{
		\local ..constText, task_btn
		match bt_name=,text=,id, bt_record\\{
			..constText cwstr text
			task_btn equ id, ..constText
		\\}
		\common
		name#.radioBtnArray @const make@task_buttons task_btn
	rept 0\{\}rept 1\{
		name#.radioBtnArray = NULL
	\}
	name#.TRADIO_BTN_COUNT = name#@task_radio_last_id - TASKDIALOG_FIRST_RADIO_ID

	match =name#@task_title, name#@task_title\{ rept 0\{\} match def_title, name#@task_title\{
		def_title
	\}

	match =name#@task_content, name#@task_content\{ rept 0\{\} match def_content, name#@task_content\{
		def_content
	\}

	match =name#@task_checkbox, name#@task_checkbox\{ rept 0\{\} match def_checkbox, name#@task_checkbox\{
		def_checkbox
	\}

	match =name#@task_expanded_control, name#@task_expanded_control\{ rept 0\{\} match def_expanded_control, name#@task_expanded_control\{
		def_expanded_control
	\}

	match =name#@task_expanded, name#@task_expanded\{ rept 0\{\} match def_expanded, name#@task_expanded\{
		def_expanded
	\}

	match =name#@task_header, name#@task_header\{ rept 0\{\} match def_header, name#@task_header\{
		def_header
	\}

	match =name#@task_collapsed_control, name#@task_collapsed_control\{ rept 0\{\} match def_collapsed_control, name#@task_collapsed_control\{
		def_collapsed_control
	\}

	match =name#@task_footer, name#@task_footer\{ rept 0\{\} match def_footer, name#@task_footer\{
		def_footer
	\}

	macro Make_tevents\{
		\local max_event, min_event, ..events, events_size
		min_event = TDN_FIRST
		max_event = TDN_LAST
		name#.tevents.counter = 0
		if name#.tevents.count <> 0
			..events __TEventsHeader
			rw events_size
		end if
		irpv _event, name#@TEventStack\\{
			if min_event > _event
				min_event = _event
			end if
			if max_event < _event
				max_event = _event
			end if
			store word name#.\\#_event at ..events\#.events_ + _event * 2
			name#.tevents.counter = name#.tevents.counter + 1
		\\}
		if name#.tevents.counter <> 0
			events_size = max_event - min_event
			store byte events_size at ..events\#.count_
			store byte min_event at ..events\#.first
		end if
		name#.tevents.count = name#.tevents.counter
	\}
	name#.tevents @const Make_tevents
	purge Make_tevents
}

add_specialfields taskdialog
