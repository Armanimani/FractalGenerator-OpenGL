/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDATETIME_H
#define QDATETIME_H

#include <QtCore/qstring.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qshareddata.h>

#include <limits>

#ifdef Q_OS_MAC
Q_FORWARD_DECLARE_CF_TYPE(CFDate);
#  ifdef __OBJC__
Q_FORWARD_DECLARE_OBJC_CLASS(NSDate);
#  endif
#endif

QT_BEGIN_NAMESPACE

class QTimeZone;

class Q_CORE_EXPORT QDate
{
public:
    enum MonthNameType {
        DateFormat = 0,
        StandaloneFormat
    };
private:
    Q_DECL_CONSTEXPR QDate(qint64 julianDay) : jd(julianDay) {}
public:
    Q_DECL_CONSTEXPR QDate() : jd(nullJd()) {}
    QDate(int y, int m, int d);

    Q_DECL_CONSTEXPR bool isNull() const { return !isValid(); }
    Q_DECL_CONSTEXPR bool isValid() const { return jd >= minJd() && jd <= maxJd(); }

    int year() const;
    int month() const;
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    int weekNumber(int *yearNum = 0) const;

#ifndef QT_NO_TEXTDATE
    static QString shortMonthName(int month, MonthNameType type = DateFormat);
    static QString shortDayName(int weekday, MonthNameType type = DateFormat);
    static QString longMonthName(int month, MonthNameType type = DateFormat);
    static QString longDayName(int weekday, MonthNameType type = DateFormat);
#endif // QT_NO_TEXTDATE
#ifndef QT_NO_DATESTRING
    QString toString(Qt::DateFormat f = Qt::TextDate) const;
    QString toString(const QString &format) const;
#endif
#if QT_DEPRECATED_SINCE(5,0)
QT_DEPRECATED inline bool setYMD(int y, int m, int d)
{ if (uint(y) <= 99) y += 1900; return setDate(y, m, d); }
#endif

    bool setDate(int year, int month, int day);

    void getDate(int *year, int *month, int *day);

    QDate addDays(qint64 days) const Q_REQUIRED_RESULT;
    QDate addMonths(int months) const Q_REQUIRED_RESULT;
    QDate addYears(int years) const Q_REQUIRED_RESULT;
    qint64 daysTo(const QDate &) const;

    Q_DECL_CONSTEXPR bool operator==(const QDate &other) const { return jd == other.jd; }
    Q_DECL_CONSTEXPR bool operator!=(const QDate &other) const { return jd != other.jd; }
    Q_DECL_CONSTEXPR bool operator< (const QDate &other) const { return jd <  other.jd; }
    Q_DECL_CONSTEXPR bool operator<=(const QDate &other) const { return jd <= other.jd; }
    Q_DECL_CONSTEXPR bool operator> (const QDate &other) const { return jd >  other.jd; }
    Q_DECL_CONSTEXPR bool operator>=(const QDate &other) const { return jd >= other.jd; }

    static QDate currentDate();
#ifndef QT_NO_DATESTRING
    static QDate fromString(const QString &s, Qt::DateFormat f = Qt::TextDate);
    static QDate fromString(const QString &s, const QString &format);
#endif
    static bool isValid(int y, int m, int d);
    static bool isLeapYear(int year);

    static Q_DECL_CONSTEXPR inline QDate fromJulianDay(qint64 jd)
    { return jd >= minJd() && jd <= maxJd() ? QDate(jd) : QDate() ; }
    Q_DECL_CONSTEXPR inline qint64 toJulianDay() const { return jd; }

private:
    // using extra parentheses around min to avoid expanding it if it is a macro
    static Q_DECL_CONSTEXPR inline qint64 nullJd() { return (std::numeric_limits<qint64>::min)(); }
    static Q_DECL_CONSTEXPR inline qint64 minJd() { return Q_INT64_C(-784350574879); }
    static Q_DECL_CONSTEXPR inline qint64 maxJd() { return Q_INT64_C( 784354017364); }

    qint64 jd;

    friend class QDateTime;
    friend class QDateTimePrivate;
#ifndef QT_NO_DATASTREAM
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QDate &);
    friend Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QDate &);
#endif
};
Q_DECLARE_TYPEINFO(QDate, Q_MOVABLE_TYPE);

class Q_CORE_EXPORT QTime
{
    Q_DECL_CONSTEXPR QTime(int ms) : mds(ms)
#if defined(Q_OS_WINCE)
        , startTick(NullTime)
#endif
    {}
public:
    Q_DECL_CONSTEXPR QTime(): mds(NullTime)
#if defined(Q_OS_WINCE)
        , startTick(NullTime)
#endif
    {}
    QTime(int h, int m, int s = 0, int ms = 0);

    Q_DECL_CONSTEXPR bool isNull() const { return mds == NullTime; }
    bool isValid() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;
#ifndef QT_NO_DATESTRING
    QString toString(Qt::DateFormat f = Qt::TextDate) const;
    QString toString(const QString &format) const;
#endif
    bool setHMS(int h, int m, int s, int ms = 0);

    QTime addSecs(int secs) const Q_REQUIRED_RESULT;
    int secsTo(const QTime &) const;
    QTime addMSecs(int ms) const Q_REQUIRED_RESULT;
    int msecsTo(const QTime &) const;

    Q_DECL_CONSTEXPR bool operator==(const QTime &other) const { return mds == other.mds; }
    Q_DECL_CONSTEXPR bool operator!=(const QTime &other) const { return mds != other.mds; }
    Q_DECL_CONSTEXPR bool operator< (const QTime &other) const { return mds <  other.mds; }
    Q_DECL_CONSTEXPR bool operator<=(const QTime &other) const { return mds <= other.mds; }
    Q_DECL_CONSTEXPR bool operator> (const QTime &other) const { return mds >  other.mds; }
    Q_DECL_CONSTEXPR bool operator>=(const QTime &other) const { return mds >= other.mds; }

    static Q_DECL_CONSTEXPR inline QTime fromMSecsSinceStartOfDay(int msecs) { return QTime(msecs); }
    Q_DECL_CONSTEXPR inline int msecsSinceStartOfDay() const { return mds == NullTime ? 0 : mds; }

    static QTime currentTime();
#ifndef QT_NO_DATESTRING
    static QTime fromString(const QString &s, Qt::DateFormat f = Qt::TextDate);
    static QTime fromString(const QString &s, const QString &format);
#endif
    static bool isValid(int h, int m, int s, int ms = 0);

    void start();
    int restart();
    int elapsed() const;
private:
    enum TimeFlag { NullTime = -1 };
    Q_DECL_CONSTEXPR inline int ds() const { return mds == -1 ? 0 : mds; }
    int mds;
#if defined(Q_OS_WINCE)
    int startTick;
#endif

    friend class QDateTime;
    friend class QDateTimePrivate;
#ifndef QT_NO_DATASTREAM
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QTime &);
    friend Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QTime &);
#endif
};
Q_DECLARE_TYPEINFO(QTime, Q_MOVABLE_TYPE);

class QDateTimePrivate;

class Q_CORE_EXPORT QDateTime
{
public:
    QDateTime();
    explicit QDateTime(const QDate &);
    QDateTime(const QDate &, const QTime &, Qt::TimeSpec spec = Qt::LocalTime);
    // ### Qt 6: Merge with above with default offsetSeconds = 0
    QDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec, int offsetSeconds);
#ifndef QT_BOOTSTRAPPED
    QDateTime(const QDate &date, const QTime &time, const QTimeZone &timeZone);
#endif // QT_BOOTSTRAPPED
    QDateTime(const QDateTime &other);
    ~QDateTime();

    QDateTime &operator=(const QDateTime &other);

    inline void swap(QDateTime &other) { qSwap(d, other.d); }

    bool isNull() const;
    bool isValid() const;

    QDate date() const;
    QTime time() const;
    Qt::TimeSpec timeSpec() const;
    int offsetFromUtc() const;
#ifndef QT_BOOTSTRAPPED
    QTimeZone timeZone() const;
#endif // QT_BOOTSTRAPPED
    QString timeZoneAbbreviation() const;
    bool isDaylightTime() const;

    qint64 toMSecsSinceEpoch() const;
    // ### Qt 6: use quint64 instead of uint
    uint toTime_t() const;

    void setDate(const QDate &date);
    void setTime(const QTime &time);
    void setTimeSpec(Qt::TimeSpec spec);
    void setOffsetFromUtc(int offsetSeconds);
#ifndef QT_BOOTSTRAPPED
    void setTimeZone(const QTimeZone &toZone);
#endif // QT_BOOTSTRAPPED
    void setMSecsSinceEpoch(qint64 msecs);
    // ### Qt 6: use quint64 instead of uint
    void setTime_t(uint secsSince1Jan1970UTC);

#ifndef QT_NO_DATESTRING
    QString toString(Qt::DateFormat f = Qt::TextDate) const;
    QString toString(const QString &format) const;
#endif
    QDateTime addDays(qint64 days) const Q_REQUIRED_RESULT;
    QDateTime addMonths(int months) const Q_REQUIRED_RESULT;
    QDateTime addYears(int years) const Q_REQUIRED_RESULT;
    QDateTime addSecs(qint64 secs) const Q_REQUIRED_RESULT;
    QDateTime addMSecs(qint64 msecs) const Q_REQUIRED_RESULT;

    QDateTime toTimeSpec(Qt::TimeSpec spec) const;
    inline QDateTime toLocalTime() const { return toTimeSpec(Qt::LocalTime); }
    inline QDateTime toUTC() const { return toTimeSpec(Qt::UTC); }
    QDateTime toOffsetFromUtc(int offsetSeconds) const;
#ifndef QT_BOOTSTRAPPED
    QDateTime toTimeZone(const QTimeZone &toZone) const;
#endif // QT_BOOTSTRAPPED

    qint64 daysTo(const QDateTime &) const;
    qint64 secsTo(const QDateTime &) const;
    qint64 msecsTo(const QDateTime &) const;

    bool operator==(const QDateTime &other) const;
    inline bool operator!=(const QDateTime &other) const { return !(*this == other); }
    bool operator<(const QDateTime &other) const;
    inline bool operator<=(const QDateTime &other) const { return !(other < *this); }
    inline bool operator>(const QDateTime &other) const { return other < *this; }
    inline bool operator>=(const QDateTime &other) const { return !(*this < other); }

#if QT_DEPRECATED_SINCE(5, 2)
    QT_DEPRECATED void setUtcOffset(int seconds);
    QT_DEPRECATED int utcOffset() const;
#endif // QT_DEPRECATED_SINCE

    static QDateTime currentDateTime();
    static QDateTime currentDateTimeUtc();
#ifndef QT_NO_DATESTRING
    static QDateTime fromString(const QString &s, Qt::DateFormat f = Qt::TextDate);
    static QDateTime fromString(const QString &s, const QString &format);
#endif
    // ### Qt 6: use quint64 instead of uint
    static QDateTime fromTime_t(uint secsSince1Jan1970UTC);
    // ### Qt 6: Merge with above with default spec = Qt::LocalTime
    static QDateTime fromTime_t(uint secsSince1Jan1970UTC, Qt::TimeSpec spec,
                                int offsetFromUtc = 0);
#ifndef QT_BOOTSTRAPPED
    static QDateTime fromTime_t(uint secsSince1Jan1970UTC, const QTimeZone &timeZone);
#endif
    static QDateTime fromMSecsSinceEpoch(qint64 msecs);
    // ### Qt 6: Merge with above with default spec = Qt::LocalTime
    static QDateTime fromMSecsSinceEpoch(qint64 msecs, Qt::TimeSpec spec, int offsetFromUtc = 0);
#ifndef QT_BOOTSTRAPPED
    static QDateTime fromMSecsSinceEpoch(qint64 msecs, const QTimeZone &timeZone);
#endif
    static qint64 currentMSecsSinceEpoch() Q_DECL_NOTHROW;

#if defined(Q_OS_MAC) || defined(Q_QDOC)
    static QDateTime fromCFDate(CFDateRef date);
    CFDateRef toCFDate() const Q_DECL_CF_RETURNS_RETAINED;
#  if defined(__OBJC__) || defined(Q_QDOC)
    static QDateTime fromNSDate(const NSDate *date);
    NSDate *toNSDate() const Q_DECL_NS_RETURNS_AUTORELEASED;
#  endif
#endif

private:
    friend class QDateTimePrivate;

    // ### Qt6: Using a private here has high impact on runtime
    // on users such as QFileInfo. In Qt 6, the data members
    // should be inlined.
    QSharedDataPointer<QDateTimePrivate> d;

#ifndef QT_NO_DATASTREAM
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QDateTime &);
    friend Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QDateTime &);
#endif

#if !defined(QT_NO_DEBUG_STREAM) && !defined(QT_NO_DATESTRING)
    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QDateTime &);
#endif
};
Q_DECLARE_SHARED(QDateTime)

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QDate &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QDate &);
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QTime &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QTime &);
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QDateTime &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QDateTime &);
#endif // QT_NO_DATASTREAM

#if !defined(QT_NO_DEBUG_STREAM) && !defined(QT_NO_DATESTRING)
Q_CORE_EXPORT QDebug operator<<(QDebug, const QDate &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const QTime &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const QDateTime &);
#endif

// QDateTime is not noexcept for now -- to be revised once
// timezone and calendaring support is added
Q_CORE_EXPORT uint qHash(const QDateTime &key, uint seed = 0);
Q_CORE_EXPORT uint qHash(const QDate &key, uint seed = 0) Q_DECL_NOTHROW;
Q_CORE_EXPORT uint qHash(const QTime &key, uint seed = 0) Q_DECL_NOTHROW;

QT_END_NAMESPACE

#endif // QDATETIME_H
