#pragma once
//�R�s�[�h�~
struct Noncopyable {
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};