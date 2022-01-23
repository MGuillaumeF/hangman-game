(async function() {
  exitStatus = 0;
  // get arguments of process run
  const args = process.argv.slice(2);
  // if args have good size run converting
  if (args.length === 1) {
      const [inputFile] = args;
      const fs = require('fs').promises;
      const path = require('path');
      const filename = path.resolve(process.cwd(), inputFile);

      const npmSeverityToSonar = new Map([
          ["info", 'INFO'],
          ["low", 'MINOR'],
          ["moderate", 'MINOR'],
          ["high", 'MAJOR'],
          ['critical', 'CRITICAL']
      ]);

      let auditJsonString = '';
      try {
          auditJsonString = await fs.readFile(filename);
          auditJsonString = auditJsonString.toString();

      } catch (error) {
          console.error('input file read failed', error);
          exitStatus = 3;
          process.exit(3);
      }

      const audit = JSON.parse(auditJsonString);
      const issues = [];
      const engineId = `npm-audit-{audit.auditReportVersion}`;
      for (const [packageName, vulnerability] of Object.entries(audit.vulnerabilities)) {
          let startLine = 1;
          if (vulnerability.isDirect) {
              const packageNameIndex = auditJsonString.findIndex(packageName);
              startLine = auditJsonString.slice(0, packageNameIndex).split('\n').length;
          }
          issues.push({
              engineId,
              ruleId : vulnerability.isDirect ? 'direct-dependency-vulnerability' : 'dependency-vulnerability',
              severity : npmSeverityToSonar.get(vulnerability.severity),
              type : 'VULNERABILITY',
              primaryLocation : {
                  message : `The dependency ${packageName} has vulnerability${vulnerability.fixAvailable ? `, fix available in ${vulnerability.fixAvailable.name} version : ${vulnerability.fixAvailable.version}` : ''}`,
                  filePath : path.resolve(process.cwd(), 'package.json'),
                  textRange : {
                      startLine,
                      startColumn : 0
                  }
              }
          });
      }
         
      const output = JSON.stringify({issues}, null, 4);
      console.debug('issues generated', output);
      try {
          await fs.writeFile(path.resolve(process.cwd(), 'dist/reports/audit-report.json'), output);
      } catch (error) {
          console.error('output file write failed', error);
          exitStatus = 2;
      }
  } else {
      console.error('one argument expected,', args.length, 'found');
      exitStatus = 1;
  }
  process.exit(exitStatus);
})();
