/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#include "info/info_common_groups_widget.h"

#include "info/info_common_groups_inner_widget.h"
#include "ui/widgets/scroll_area.h"

namespace Info {
namespace CommonGroups {

object_ptr<ContentWidget> Memento::createWidget(
		QWidget *parent,
		rpl::producer<Wrap> wrap,
		not_null<Window::Controller*> controller,
		const QRect &geometry) {
	auto result = object_ptr<Widget>(
		parent,
		std::move(wrap),
		controller,
		App::user(userId()));
	result->setInternalState(geometry, this);
	return std::move(result);
}

Widget::Widget(
	QWidget *parent,
	rpl::producer<Wrap> wrap,
	not_null<Window::Controller*> controller,
	not_null<UserData*> user)
: ContentWidget(parent, wrap, controller, user) {
	_inner = setInnerWidget(object_ptr<InnerWidget>(this, user));
}

not_null<UserData*> Widget::user() const {
	return _inner->user();
}

Section Widget::section() const {
	return Section(Section::Type::CommonGroups);
}

bool Widget::showInternal(not_null<ContentMemento*> memento) {
	if (auto groupsMemento = dynamic_cast<Memento*>(memento.get())) {
		if (groupsMemento->userId() == user()->bareId()) {
			restoreState(groupsMemento);
			return true;
		}
	}
	return false;
}

void Widget::setInternalState(const QRect &geometry, not_null<Memento*> memento) {
	setGeometry(geometry);
	myEnsureResized(this);
	restoreState(memento);
}

std::unique_ptr<ContentMemento> Widget::createMemento() {
	auto result = std::make_unique<Memento>(user()->bareId());
	saveState(result.get());
	return std::move(result);
}

void Widget::saveState(not_null<Memento*> memento) {
	memento->setScrollTop(scrollTopSave());
	_inner->saveState(memento);
}

void Widget::restoreState(not_null<Memento*> memento) {
	_inner->restoreState(memento);
	auto scrollTop = memento->scrollTop();
	scrollTopRestore(memento->scrollTop());
	// TODO is setVisibleTopBottom called?
}

} // namespace CommonGroups
} // namespace Info
