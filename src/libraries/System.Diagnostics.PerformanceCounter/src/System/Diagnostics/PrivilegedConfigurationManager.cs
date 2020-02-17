// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

namespace System.Configuration {

    using System.Collections.Specialized;
    using System.Security;

    internal static class PrivilegedConfigurationManager {
        internal static ConnectionStringSettingsCollection ConnectionStrings {
            get {
                return ConfigurationManager.ConnectionStrings;
            }
        }

        internal static object GetSection(string sectionName) {
            return ConfigurationManager.GetSection(sectionName);
        }
    }
}
