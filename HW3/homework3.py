#Final no recursion loops detection by thresh hold
import copy

#MAX_DEPTH = 1200
#MAX_CLAUSE = 500
LOOP_THRESH = 100

def dbg(x,y=None):
    return
    print x
    if not y==None:
        y()

def dbgT(x):
    return
    print x

def dbgProof(x):
    return
    print x

class Clause:
    def __init__(s,num,st=''):
        s.predicates = []
        s.predicateLocations = {}
        if not st == '':
            s.parse(st) #will parse the list of all predicates in the clause
        s.number = num #each clause should have a unique number
        s.resolved = False #Once this statement is used against all others for resolving, we will say that it is resolved

    def parse(s,st):
        s.predicates = []
        st = st.strip()
        l = st.split('|')
        for part in l:
            part = part.strip()
            p = Predicate(part)
            s.predicates.append(p)
        s.predicates.sort()
        s.buildDict()

    def buildDict(s): #populates the predicate locations
        s.predicateLocations = {}
        for i in range(0,len(s.predicates)):
            p = s.predicates[i]
            if not (p.name,p.truth) in s.predicateLocations:
                s.predicateLocations[(p.name,True)] = []
                s.predicateLocations[(p.name,False)] = []
            s.predicateLocations[(p.name,p.truth)].append(i)

    def printClause(s):
        print "Clause #%d"%(s.number)
        for p in s.predicates:
            p.printPredicate()
        print

    def prettyprint(s):
        if len(s.predicates)==0:
            return "fi"
        sentence = "null"
        for p in s.predicates:
            st = p.prettyprint()
            if sentence=="null":
                sentence = st
            else:
                sentence = sentence + " | " + st
        
        return sentence
            

            
class Predicate:
    def __init__(s,st):
        s.parse(st)

    def __cmp__(self,other):
        return cmp(self.name,other.name)

    def printPredicate(s):
        print s.name,s.truth,s.args

    def parse(s,st):
        st = st.strip()
        l = st.split('(')
        key = l[0]
        if key[0] == '~':
            key = key[1:]
            truth = False
        else:
            truth = True
        l = l[1][:-1]
        l = l.split(',')

        s.name = key
        s.truth = truth
        s.args = l

    def prettyprint(s):
        name = s.name
        if s.truth == False:
            name = '~'+name
        name = name + '('
        for a in s.args:
            name = name + a
            name = name + ','
        name = name[:-1]+')'
        return name


def isConstant(arg):
    return arg[0].isupper() #Considering _ as first character to mean a variable

def inputQueries(nq):   #Takes nq query statements as input, which are all single predicates
    for i in range(0,nq):
        s = fin.readline()
        s = s.strip()
        p = Clause(-1,s)    #creates a new predicate by parsing the given string
        queries.append(p)

def inputStatements(ns):
    for i in range(0,ns):
        s = fin.readline()
        s = s.strip()
        c = Clause(i,s)
        clauses.append(c)

def modifyVariables(c): #Take all variables of a clause and add a _ before them, _ returns false for isupper and islower
    for p in c.predicates:
        for i in range(0,len(p.args)):
            a = p.args[i]
            if not isConstant(a):
                p.args[i] = '_'+a

def modifyBackVariables(c):
    for p in c.predicates:
        for i in range(0,len(p.args)):
            a = p.args[i]
            if a[0]=='_':
                p.args[i] = p.args[i][1:]

def tryUnify(p1,p2): #try to unify two predicates, if they do unify, return a substitution list, otherwise empty dict
    sigma = {}
    gotSomeConstants = False
    for a1,a2 in zip(p1.args,p2.args):
        if isConstant(a1) and isConstant(a2):   #if both are constants
            if not a1 == a2: #Only makes sense if both constants are the same
                return {}   #otherwise can't unify
            else:
                gotSomeConstants = True
        elif isConstant(a1) and not isConstant(a2): #P(John) P(_x)
            if a2 not in sigma:
                sigma[a2] = a1
            elif not sigma[a2] == a1:
                return {}
        elif not isConstant(a1) and isConstant(a2): #P(x) P(John)
            if a1 not in sigma:
                sigma[a1] = a2
            elif not sigma[a1] == a2:
                return {}
        elif not isConstant(a1) and not isConstant(a2): #P(x) P(_x)
            if a2 not in sigma:
                sigma[a2] = a1
            elif not sigma[a2] == a1:
                return {}

    if gotSomeConstants and len(sigma)==0:
        sigma["Constants"] = True
    return sigma

def renameVariables(c): #rename variables like _x to some unique variable
    sigma = {}
    names = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']
    index = 0
    for p in c.predicates:
        for a in p.args:
            sigma[a] = a

    for p in c.predicates:  #We will go through all the args and create our 
        for i in range(0,len(p.args)):
            a = p.args[i]
            if a[0] == '_' and sigma[a] == a:   #haven't set a replacement value for the 2nd clause variable yet
                while index<26 and names[index] in sigma:
                    index = index + 1
                if index == 26:
                    j = 0
                    orig = a[1:]
                    newName = orig+str(j)
                    while newName in sigma:
                        j = j+1
                        newName = orig+str(j)
                else:
                    newName = names[index]
                    index = index + 1
                sigma[a] = newName #replacement value set

    for p in c.predicates:
        for i in range(0,len(p.args)):
            p.args[i] = sigma[p.args[i]]    #replace all variable names with new ones (simple ones like x will just be replaced with x)

def standardisePredicate(predi):
    sigma = {}
    names = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']
    index = 0
    
    for i in range(0,len(predi.args)):
        a = predi.args[i]
        if not isConstant(a) and not a in sigma:
            sigma[a] = names[index]
            index = index+1
        elif isConstant(a):
            sigma[a] = a
            
    for i in range(0,len(predi.args)):
        predi.args[i] = sigma[predi.args[i]]
"""
def search(c,depth):    #searches if clause c can be resolved recursively, depth used to kill infinite loops
    if depth >= MAX_DEPTH:
        dbg("Max depth reached, backtracking")
        return False
    #dbgT(depth)

    dbg("Starting search for "+c.prettyprint())
    firstPredicate = c.predicates[0]
    dbg("First predicate is "+firstPredicate.prettyprint())
    
    fp = firstPredicate
    if (fp.name,not fp.truth) in predicatePositions:
        matches = predicatePositions[(fp.name,not fp.truth)]
    else:
        return False

    dbg(("Matched with clauses",matches))
    for m in matches:
        dbg("trying "+clauses[m].prettyprint())
        c2 = clauses[m] #some potential clause with whom we can unify
        
        cp1 = c#copy.deepcopy(c)
        cp2 = c2#copy.deepcopy(c2)
        modifyVariables(cp2) #guarantees no conflict between variables of clause1 and clause2
        newClauses = []
        tryResolve(cp1,cp2,{},[[],[]],0,newClauses)  #try resolving, if it works, then we get some clause
        modifyBackVariables(cp2)
        
        worked = not len(newClauses) == 0 #we got some clauses back
                
        if worked:  #we got a new clause
            for newClause in newClauses:
                if len(newClause.predicates) == 0: #implies contradiction
                    dbgProof(c.prettyprint()+' with '+c2.prettyprint()+' :=> '+newClause.prettyprint())
                    return True
                elif search(newClause,depth+1):
                    dbgProof(c.prettyprint()+' with '+c2.prettyprint()+' :=> '+newClause.prettyprint())
                    return True
            del newClauses
        else:
            dbg("Got no clauses")


    return False
"""

def searchi(cl):
    usedPredicates = {}
    stack = []
    stack.append((copy.deepcopy(cl),0)) #clause and depth
    while not len(stack) == 0:
        if type(stack[-1]) is str:#string entered
            predicate = stack[-1][:-1]
            stringtype = stack[-1][-1]
            if stringtype == 'a':
                usedPredicates[predicate] = usedPredicates[predicate]+1
            else: #originalPredicate
                usedPredicates.pop(predicate)   #pop out the used predicate, all branches are over
            dbg(usedPredicates)
            stack.pop()
            continue
        
        c = stack[-1][0]
        depth = stack[-1][1]

        stack.pop()

        """
        if depth >= MAX_DEPTH:
            dbg("Max depth reached, backtracking")
            continue
        dbgT(depth)
        """
        dbg("Starting search for "+c.prettyprint())
        firstPredicate = c.predicates[0]

        sp = copy.deepcopy(firstPredicate)
        standardisePredicate(sp)
        spkey = sp.prettyprint()
        dbg("First predicate is "+firstPredicate.prettyprint())

        if spkey in usedPredicates:
            if usedPredicates[spkey]==0:
                dbg("Found loop, returning false")
                continue
            else:
                usedPredicates[spkey] = usedPredicates[spkey] - 1
                stack.append(spkey+'a')
        else:
            """
            c.buildDict()
            num = 0
            matches = c.predicateLocations[(sp.name,sp.truth)]
            for i in matches: #Will match with atleast the firstPredicate
                spc = copy.deepcopy(c.predicates[i])
                standardisePredicate(spc)
                spckey = spc.prettyprint()
                if spckey == spkey: #So this will match at least once
                    num = num + 1
                del spc
            num = num - 1
            usedPredicates[spkey] = num
            """
            usedPredicates[spkey] = LOOP_THRESH
            dbg(usedPredicates)
            stack.append(spkey+'o')
        
        
        fp = firstPredicate
        if (fp.name,not fp.truth) in predicatePositions:
            matches = predicatePositions[(fp.name,not fp.truth)]
        else:
            continue

        dbg(("Matched with clauses",matches))
        for m in matches:
            dbg("trying "+clauses[m].prettyprint())
            c2 = clauses[m] #some potential clause with whom we can unify
            
            cp1 = c#copy.deepcopy(c)
            cp2 = c2#copy.deepcopy(c2)
            modifyVariables(cp2) #guarantees no conflict between variables of clause1 and clause2
            newClauses = []
            tryResolvei(cp1,cp2,{},[[],[]],0,newClauses)  #try resolving, if it works, then we get some clause
            modifyBackVariables(cp2)
            
            worked = not len(newClauses) == 0 #we got some clauses back
                    
            if worked:  #we got a new clause
                for newClause in newClauses:
                    if len(newClause.predicates) == 0: #implies contradiction
                        dbgProof(c.prettyprint()+' with '+c2.prettyprint()+' :=> '+newClause.prettyprint())
                        return True
                    else:
                        for nc in newClauses:
                            stack.append((copy.deepcopy(nc),depth+1))
                del newClauses
            else:
                dbg("Got no clauses")
                continue


    return False

"""
def tryResolve(c1,c2,sigma,pignore,pnum,newClauses):
    if pnum == len(c1.predicates): #Base case!
        #We're finally here, now make a new clause
        makeNewClause(c1,c2,sigma,pignore,newClauses)
        return

    #See if there are any matches for the predicate we are on, if yes make a list of positions, otherwise recurse for the next one
    p1 = c1.predicates[pnum]
    if (p1.name,not p1.truth) in c2.predicateLocations and len(c2.predicateLocations[(p1.name,not p1.truth)])>0:    #If matches are found for this predicate of clause1 in clause2
        pmatches = c2.predicateLocations[(p1.name,not p1.truth)]
    else:   #Otherwise we will just recurse to use the next predicate of clause1
        tryResolve(c1,c2,sigma,pignore,pnum+1,newClauses)
        return

    recursed = False
    for p2num in pmatches:
        ret = tryUnify(p1,c2.predicates[p2num])
        dbg(ret)
        if ret == {}:
            continue
        else:   #unification worked, we have the substitution list(dict) in ret
            if not "Constants" in ret: #We have some variables to substitute
                #Check if new substitutions are compatible with sigma
                compatible = True
                for k in ret:
                    if k in sigma:
                        if not ret[k] == sigma[k]:  #If a variable is asigned a different variable or constant, we can't work with this new substitution
                            compatible = False
                            break
                if compatible: #if compatible, update sigma
                    sigmacpy = sigma.copy() #save sigma to restore later
                    for k in ret:
                        sigma[k] = ret[k] #New values are inserted, old are overwritten with the same values
                else:
                    continue
            else: #We only have constants, and it still unified!
                sigmacpy = sigma.copy()
                

            #If we're here, then the compatibility checked out, now to add these predicates to the pignore
            pignore[0].append(pnum)
            pignore[1].append(p2num)

            #We should now have the pignore as well as the sigma updated, so now we recurse
            tryResolve(c1,c2,sigma,pignore,pnum+1,newClauses)
            recursed = True

            #Now we're out of the recursion, should reset sigma and pignore
            sigma = sigmacpy
            del sigmacpy
            pignore[0].pop()
            pignore[1].pop()

    if not recursed:
        tryResolve(c1,c2,sigma,pignore,pnum+1,newClauses)
"""

def tryResolvei(c1,c2,sigmao,pignoreo,pnumo,newClauses):
    #c1, c2, newClauses won't go in stack
    stack = []
    dc = copy.deepcopy
    stack.append([dc(sigmao),dc(pignoreo),dc(pnumo)])

    while not len(stack) == 0:

        sigma,pignore,pnum = stack[-1][0],stack[-1][1],stack[-1][2]
        stack.pop()
        
        if pnum == len(c1.predicates): #Base case!
            #We're finally here, now make a new clause
            makeNewClause(c1,c2,sigma,pignore,newClauses)
            continue

        #See if there are any matches for the predicate we are on, if yes make a list of positions, otherwise recurse for the next one
        p1 = c1.predicates[pnum]
        if (p1.name,not p1.truth) in c2.predicateLocations and len(c2.predicateLocations[(p1.name,not p1.truth)])>0:    #If matches are found for this predicate of clause1 in clause2
            pmatches = c2.predicateLocations[(p1.name,not p1.truth)]
        else:   #Otherwise we will just recurse to use the next predicate of clause1
            stack.append([dc(sigma),dc(pignore),dc(pnum+1)])
            continue

        recursed = False
        for p2num in pmatches:
            ret = tryUnify(p1,c2.predicates[p2num])
            dbg(ret)
            if ret == {}:
                continue
            else:   #unification worked, we have the substitution list(dict) in ret
                if not "Constants" in ret: #We have some variables to substitute
                    #Check if new substitutions are compatible with sigma
                    compatible = True
                    for k in ret:
                        if k in sigma:
                            if not ret[k] == sigma[k]:  #If a variable is asigned a different variable or constant, we can't work with this new substitution
                                compatible = False
                                break
                    if compatible: #if compatible, update sigma
                        sigmacpy = sigma.copy()
                        for k in ret:
                            sigma[k] = ret[k] #New values are inserted, old are overwritten with the same values
                    else:
                        continue
                else: #We only have constants, and it still unified!
                    sigmacpy = sigma.copy()
                    

                #If we're here, then the compatibility checked out, now to add these predicates to the pignore
                pignore[0].append(pnum)
                pignore[1].append(p2num)

                #We should now have the pignore as well as the sigma updated, so now we recurse
                stack.append([dc(sigma),dc(pignore),dc(pnum+1)])
                recursed = True

                sigma = sigmacpy
                pignore[0].pop()
                pignore[1].pop()

        if not recursed:
            stack.append([dc(sigma),dc(pignore),dc(pnum+1)])
            
def makeNewClause(c1,c2,sigma,pignore,newClauses):
    global ns
    
    if pignore[0] == []: #if no predicates were unified
        dbg("couldn't unify anything")
        return

    dbg(("making clause with",sigma,pignore))
    
    #If we're here, we'll make some or the other clause
    newCl = Clause(ns,'')  #new empty clause, give it number ns,
    ns = ns+1 #update ns (not needed per se, but might help with debugging)

    for k in sigma: #In case _x type variables need to be substituted by other variables, we can just put the corresponding constants in them
        if k[0]=='_' and not isConstant(sigma[k]):  #a clause2 variable has value as a clause1 variable
            if sigma[k] in sigma:
                sigma[k] = sigma[sigma[k]] #put the clause1 variable's value into the clause2 variable
                    

    #Copy the first clause predicates
    i = 0 
    for ig in pignore[0]:
        while i<ig:
            newCl.predicates.append(copy.deepcopy(c1.predicates[i]))
            i = i+1
            for j in range(0,len(newCl.predicates[-1].args)):
                a = newCl.predicates[-1].args[j]
                if a in sigma:   #we need to make a substitution for variable
                    newCl.predicates[-1].args[j] = sigma[a]
        i = i+1

    while i<len(c1.predicates): #we still have some predicates to copy after the last ignored predicate
        newCl.predicates.append(copy.deepcopy(c1.predicates[i]))
        for j in range(0,len(newCl.predicates[-1].args)):
            a = newCl.predicates[-1].args[j]
            if a in sigma:   #we need to make a substitution for variable
                newCl.predicates[-1].args[j] = sigma[a]
        i = i+1

    #Copy the second clause predicates
    i = 0 
    for ig in pignore[1]:
        while i<ig:
            newCl.predicates.append(copy.deepcopy(c2.predicates[i]))
            i = i+1
            for j in range(0,len(newCl.predicates[-1].args)):
                a = newCl.predicates[-1].args[j]
                if a in sigma:   #we need to make a substitution for variable
                    newCl.predicates[-1].args[j] = sigma[a]
        i = i+1

    while i<len(c2.predicates): #we still have some predicates to copy after the last ignored predicate
        newCl.predicates.append(copy.deepcopy(c2.predicates[i]))
        for j in range(0,len(newCl.predicates[-1].args)):
            a = newCl.predicates[-1].args[j]
            if a in sigma:   #we need to make a substitution for variable
                newCl.predicates[-1].args[j] = sigma[a]
        i = i+1

        

    #newCl.predicates.sort()
    newCl.buildDict()
    renameVariables(newCl)  #rename _x to unique variable names

    #if len(newCl.predicates)<=MAX_CLAUSE:
    newClauses.append(newCl)
    dbg(newCl.prettyprint())
    dbgT(len(newCl.predicates))
    
""" Main program starts here """

fin = open("input.txt","r")
fout = open("output.txt","w")

queries = [] #list of predicates
clauses = []
predicatePositions = {}
usedPredicates = {}

nq = int(fin.readline())
inputQueries(nq)
ns = int(fin.readline())
inputStatements(ns)

"""
#Just testing to see if all inputs are correct
for q in queries:
    q.printClause()
for c in clauses:
    c.printClause()
"""

#set up all predicate positions
for c in clauses:
    for p in c.predicates:
        if not (p.name,p.truth) in predicatePositions:
            predicatePositions[(p.name,True)] = []
            predicatePositions[(p.name,False)] = []
            
        predicatePositions[(p.name,p.truth)].append(c.number)   #can containt duplicates

for pp in predicatePositions:
    predicatePositions[pp] = set(predicatePositions[pp])


dbg("Starting Search")
for q in queries:
    q.predicates[0].truth = not q.predicates[0].truth
    q.buildDict() #originally made with different truth value during parsing
    clauses.append(q) #add negated clause to the KB
    p = q.predicates[0] 
    if not (p.name,p.truth) in predicatePositions: #If completely new predicate
        predicatePositions[(p.name,p.truth)] = set([len(clauses)-1])
    else: #if entry already exists for this predicate
        predicatePositions[(p.name,p.truth)].update([len(clauses)-1])
        
    if searchi(q):
        print "TRUE"
        fout.write("TRUE\n")
    else:
        print "FALSE"
        fout.write("FALSE\n")

    predicatePositions[(p.name,p.truth)].remove(len(clauses)-1)
    clauses.pop()

        
fin.close()
fout.close()
