typedef int (*error_handler_t)(int level, const char *filename, int line, int column, const char *message);

class DiagnosticConsumerHandlerCall : public clang::DiagnosticConsumer
{
public:
    DiagnosticConsumerHandlerCall(error_handler_t handler) :
        handler_(handler),
        aborted_(false),
        fatals_(0),
        errors_(0),
        warns_(0)
    {
    }

    virtual void HandleDiagnostic(clang::DiagnosticsEngine::Level Level, const clang::Diagnostic &Info)
    {
        if (aborted_) return;

        auto& sm = Info.getSourceManager();
        auto& sl = Info.getLocation();
        auto loc = sm.getPresumedLoc(sl);
        auto filename = loc.getFilename();
        unsigned int line = loc.getLine();
        auto column = loc.getColumn();
        clang::SmallString<256> OutStr;
        Info.FormatDiagnostic(OutStr);
        switch (Level) {
        case clang::DiagnosticsEngine::Level::Fatal:
            ++fatals_;
            break;
        case clang::DiagnosticsEngine::Level::Error:
            ++errors_;
            break;
        case clang::DiagnosticsEngine::Level::Warning:
            ++warns_;
        default:
            ;
        }
        if (handler_) {
            if (!handler_(Level, filename, line, column, OutStr.c_str())) {
                aborted_ = true;
            }
        }
    }

    bool isAborted()
    {
        return aborted_;
    }

    int getTotalErrorCount()
    {
        return fatals_ + errors_ + warns_;
    }

    int getFatalCount()
    {
        return errors_;
    }

    int getErrorCount()
    {
        return errors_;
    }

    int getWarnCount()
    {
        return warns_;
    }

private:
    error_handler_t handler_;
    bool aborted_;
    int  fatals_;
    int  errors_;
    int  warns_;
};