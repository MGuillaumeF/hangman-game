(async function(){
  const args = process.argv.slice(2);
  if (args.length === 2) {
      const fs = require('fs').promises;
      const path = require('path');
      const filename = path.resolve(__dirname, process.args.pop());
      const auditJsonString = (await fs.readFile(filename)).toString();
      const audit = JSON.parse(auditJsonString);
      const issues = [];
      for (const [packageName, vulnerability] of audit.vulnerabilies) {
          issues.push({
              engineId : 'npm-audit',
              ruleId : 'dependency-vulnerability',
              severity : vulnerability.severity,
              type : 'VULNERABILITY',
              primaryLocation : {
                  message : 'temp message',
                  filePath : path.resolve(process.cwd(), 'package.json'),
                  textRange : {
                      startLine : 1,
                      startColumn : 0
                  }
              }
          });
      }
      await fs.writeFile(path.resolve(process.cwd(), 'audit-report.json'), JSON.stringify({issues}, null, 4));
  } else {
      console.error('one argument expected', args.length, 'found'); 
  }
})();
