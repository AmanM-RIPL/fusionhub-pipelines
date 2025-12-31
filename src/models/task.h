#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Task: public QObject
{
    Q_OBJECT

    Q_PROPERTY(long long id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString taskName READ getTaskName WRITE setTaskName NOTIFY taskNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString bimElement READ getBimElement WRITE setBimElement NOTIFY bimElementChanged)
    Q_PROPERTY(QString startDate READ getStartDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QString endDate READ getEndDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(long long pid READ getParentId WRITE setParentId NOTIFY parentIdChanged)

    Q_PROPERTY(int days READ getDuration WRITE setDuration NOTIFY durationChanged)

    Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(int startDay READ getStartDay WRITE setStartDay NOTIFY startDayChanged)
    Q_PROPERTY(int endDay READ getEndDay WRITE setEndDay NOTIFY endDayChanged)

    Q_PROPERTY(int startMonth READ getStartMonth WRITE setStartMonth NOTIFY startMonthChanged)
    Q_PROPERTY(int endMonth READ getEndMonth WRITE setEndMonth NOTIFY endMonthChanged)


    Q_PROPERTY(QString jan READ getJan WRITE setJan NOTIFY janChanged)
    Q_PROPERTY(QString feb READ getFeb WRITE setFeb NOTIFY febChanged)

    Q_PROPERTY(QString mar READ getMar WRITE setMar NOTIFY marChanged)
    Q_PROPERTY(QString apr READ getApr WRITE setApr NOTIFY aprChanged)

    Q_PROPERTY(QString may READ getMay WRITE setMay NOTIFY mayChanged)
    Q_PROPERTY(QString jun READ getJun WRITE setJun NOTIFY junChanged)

    Q_PROPERTY(QString jul READ getJul WRITE setJul NOTIFY julChanged)
    Q_PROPERTY(QString aug READ getAug WRITE setAug NOTIFY augChanged)

    Q_PROPERTY(QString sep READ getSep WRITE setSep NOTIFY sepChanged)
    Q_PROPERTY(QString oct READ getOct WRITE setOct NOTIFY octChanged)

    Q_PROPERTY(QString nov READ getNov WRITE setNov NOTIFY novChanged)
    Q_PROPERTY(QString dec READ getDec WRITE setDec NOTIFY decChanged)


    Q_PROPERTY(int startYear READ getStartYear WRITE setStartYear NOTIFY startYearChanged)
    Q_PROPERTY(int endYear READ getEndYear WRITE setEndYear NOTIFY endYearChanged)

    Q_PROPERTY(QString month_year READ getMonth_Year WRITE setMonth_Year NOTIFY month_yearChanged)


public:
    explicit Task(QObject* parent = nullptr): QObject(parent) {}
    Task(long long id, const QString& globalId, bool approvalStatus,
             const QString& taskName,
             const QString& description,
             const QString& bimElement,
             const QString& startDate,
             const QString& endDate,
             long long pid,
            QObject* parent = nullptr);

    long long getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getTaskName() const { return taskName; }
    QString getDescription() const { return description; }
    QString getBimElement() const { return bimElement; }
    QString getStartDate() const { return startDate; }
    QString getEndDate() const { return endDate; }
    long long getParentId() const { return pid; }
    int getDuration() const { return days; }
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getStartDay() const { return startDay; }
    int getEndDay() const { return endDay; }
    int getStartMonth() const { return startMonth; }
    int getEndMonth() const { return endMonth; }

    QString getJan() const { return jan; }
    QString getFeb() const { return feb; }

    QString getMar() const { return mar; }
    QString getApr() const { return apr; }

    QString getMay() const { return may; }
    QString getJun() const { return jun; }

    QString getJul() const { return jul; }
    QString getAug() const { return aug; }

    QString getSep() const { return sep; }
    QString getOct() const { return oct; }

    QString getNov() const { return nov; }
    QString getDec() const { return dec; }

    int getStartYear() const { return startYear; }
    int getEndYear() const { return endYear; }

    QString getMonth_Year() const { return month_year; }


    void setId(long long id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setTaskName(const QString& taskName) { this->taskName = taskName; }
    void setDescription(const QString& description) { this->description = description; }
    void setBimElement(const QString& bimElement) { this->bimElement = bimElement; }
    void setStartDate(const QString& startDate) { this->startDate = startDate; }
    void setEndDate(const QString& endDate) { this->endDate = endDate; }
    void setParentId(long long pid) { this->pid = pid; }
    void setDuration(const int days) { this->days = days; }
    void setYear(const int year) { this->year = year; }
    void setMonth(const int month) { this->month = month; }
    void setStartDay(const int startDay) { this->startDay = startDay; }
    void setEndDay(const int endDay) { this->endDay = endDay; }

    void setStartMonth(const int startMonth) { this->startMonth = startMonth; }
    void setEndMonth(const int endMonth) { this->endMonth = endMonth; }

    void setJan(const QString& jan) { this->jan = jan; }
    void setFeb(const QString& feb) { this->feb = feb; }

    void setMar(const QString& mar) { this->mar = mar; }
    void setApr(const QString& apr) { this->apr = apr; }

    void setMay(const QString& may) { this->may = may; }
    void setJun(const QString& jun) { this->jun = jun; }

    void setJul(const QString& jul) { this->jul = jul; }
    void setAug(const QString& aug) { this->aug = aug; }

    void setSep(const QString& sep) { this->sep = sep; }
    void setOct(const QString& oct) { this->oct = oct; }

    void setNov(const QString& nov) { this->nov = nov; }
    void setDec(const QString& dec) { this->dec = dec; }

    void setStartYear(const int startYear) { this->startYear = startYear; }
    void setEndYear(const int endYear) { this->endYear = endYear; }

    void setMonth_Year(const QString& month_year) { this->month_year = month_year; }


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void taskNameChanged();
    void descriptionChanged();
    void bimElementChanged();
    void startDateChanged();
    void endDateChanged();
    void parentIdChanged();
    void durationChanged();

    void yearChanged();
    void monthChanged();
    void startDayChanged();
    void endDayChanged();

    void startMonthChanged();
    void endMonthChanged();

    void janChanged();
    void febChanged();

    void marChanged();
    void aprChanged();

    void mayChanged();
    void junChanged();

    void julChanged();
    void augChanged();

    void sepChanged();
    void octChanged();

    void novChanged();
    void decChanged();

    void startYearChanged();
    void endYearChanged();
    void month_yearChanged();

private:
    long long id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString taskName;
    QString description;
    QString bimElement;
    QString startDate;
    QString endDate;
    int days = 0;
    long long pid = 0;

    int year = 0;
    int month = 0;
    int startDay = 0;
    int endDay = 0;

    int startMonth = 0;
    int endMonth = 0;

    QString jan = "";
    QString feb = "";
    QString mar = "";
    QString apr = "";
    QString may = "";
    QString jun = "";
    QString jul = "";
    QString aug = "";
    QString sep = "";
    QString oct = "";
    QString nov = "";
    QString dec = "";    

    int startYear = 0;
    int endYear = 0;
    QString month_year = "";

};

Q_DECLARE_METATYPE(Task)

#endif // TASK_H

