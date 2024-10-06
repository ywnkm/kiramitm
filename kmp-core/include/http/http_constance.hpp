#pragma once

namespace kmp::http {

enum class status : int {

            // 1xx
            Continue = 100,
            SwitchingProtocols = 101,
            Processing = 102,
            EarlyHints = 103,

            // 2xx
            OK = 200,
            Created = 201,
            Accepted = 202,
            NonAuthoritativeInformation = 203,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,
            MultiStatus = 207,
            AlreadyReported = 208,
            IMUsed = 226,

            // 3xx
            MultipleChoices = 300,
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            // UseProxy = 305,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,

            // 4xx
            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            ProxyAuthenticationRequired = 407,
            RequestTimeout = 408,
            Conflict = 409,
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            PayloadTooLarge = 413,
            URITooLong = 414,
            UnsuppotedMediaType = 415,
            RangeNotSatisfiable = 416,
            ExpectationFailed = 417,
            ImATeapot = 418,
            MisdirectedRequest = 421,
            UnprocessableEntity = 422,
            Locked = 423,
            FailedDependency = 424,
            TooEarly = 425,
            UpgradeRequired = 426,
            PreconditionRequired = 428,
            TooManyRequests = 429,
            RequestHeaderFieldsTooLarge = 431,
            UnavailableForLegalReasons = 451,

            // 5xx
            InternalServerError = 500,
            NotImplemented = 501,
            BadGateway = 502,
            ServiceUnawailable = 503,
            GatewayTimeout = 504,
            HTTPVersionNotSupported = 505,
            VariantAlsoNegotiates = 506,
            InsufficientStorage = 507,
            NotExtended = 510,
            NetworkAuthenticationRequired = 511,
        };

        namespace method {
            constexpr char GET[] = "GET";
            constexpr char POST[] = "POST";
            constexpr char PUT[] = "PUT";
            // constexpr char DELETE[] = "DELETE"; // msvc error C2059 ???
            constexpr char HEAD[] = "HEAD";
            constexpr char OPTIONS[] = "OPTIONS";
            constexpr char CONNECT[] = "CONNECT";
            constexpr char TRACE[] = "TRACE";
            constexpr char PATCH[] = "PATCH";

        }

        namespace version {
            constexpr char HTTP_1_0[] = "HTTP/1.0";
            constexpr char HTTP_1_1[] = "HTTP/1.1";
            constexpr char HTTP_2_0[] = "HTTP/2.0";

        }

        constexpr char ConnectionEstablished11[] = "HTTP/1.1 200 Connection established\r\n\r\n";

} // namespace kmp::http
